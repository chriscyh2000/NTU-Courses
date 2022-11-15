from argparse import ArgumentParser, Namespace
from pathlib import Path
import math
import logging
from datasets import load_metric
from dataset import MC_Dataset

import torch
from torch.utils.data import DataLoader
from tqdm import tqdm

from transformers import (
    BertForMultipleChoice,
    BertConfig,
    AutoTokenizer,
    default_data_collator,
    get_scheduler,
    SchedulerType
)
from accelerate import Accelerator

# os.environ["CUDA_VISIBLE_DEVICES"] = "1,2,3"

logging.basicConfig(
    format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
    datefmt="%m/%d/%Y %H:%M:%S",
    level=logging.INFO,
)
logger = logging.getLogger(__name__)

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--data_dir",
        type=Path,
        help="Directory to the dataset.",
        default="./data/"
    )
    parser.add_argument(
        "--cache_dir",
        type=Path,
        help="Directory to the preprocessed caches.",
        default="./cache/mc/",
    )
    parser.add_argument(
        "--pretrained_model_path",
        type=Path,
        help="Directory to the pretrained model.",
        default="./ckpt/pretrained/roberta/",
    )
    parser.add_argument(
        "--model_save_dir",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpt/trained/mc_for_report/",
    )

    # optimizer
    parser.add_argument("--lr", type=float, default=5e-5)
    parser.add_argument("--weight_decay", type=float, default=0.0, help="Weight decay to use.")

    # data loader
    parser.add_argument(
        "--per_device_train_batch_size",
        type=int,
        default=1,
        help="Batch size (per device) for the training dataloader.",
    )
    parser.add_argument(
        "--per_device_val_batch_size",
        type=int,
        default=1,
        help="Batch size (per device) for the validation dataloader.",
    )
    parser.add_argument("--accumulate_step", type=int, default=16)

    # training
    parser.add_argument("--num_epoch", type=int, default=1)
    parser.add_argument(
        "--lr_scheduler_type",
        type=SchedulerType,
        default="polynomial",
        help="The scheduler type to use.",
        choices=["linear", "cosine", "cosine_with_restarts", "polynomial", "constant", "constant_with_warmup"],
    )
    parser.add_argument(
        "--num_warmup_steps", type=int, default=0, help="Number of steps for the warmup in the lr scheduler."
    )

    args = parser.parse_args()
    return args

def main(args):
    accelerator = Accelerator()
    device = accelerator.device

    # Model
    config = BertConfig(hidden_size=32, num_attention_heads=8, pooler_num_fc_layers=1)
    model = BertForMultipleChoice(config)
    model.to(device)

    tokenizer = AutoTokenizer.from_pretrained(args.pretrained_model_path, use_fast=True)
    model.resize_token_embeddings(len(tokenizer))

    # Dataset
    with accelerator.main_process_first(): 
        train_dataset_file = args.cache_dir / "train_token.json"
        train_dataset = MC_Dataset(train_dataset_file)

        valid_dataset_file = args.cache_dir / "valid_token.json"
        valid_dataset = MC_Dataset(valid_dataset_file)

    # DataLoader
    train_loader = DataLoader(
        train_dataset, shuffle=True, collate_fn=default_data_collator, batch_size=args.per_device_train_batch_size
    )
    valid_loader = DataLoader(
        valid_dataset, shuffle=False, collate_fn=default_data_collator, batch_size=args.per_device_val_batch_size
    )
    
    # Optimizer
    no_decay = ["bias", "LayerNorm.weight"]
    optimizer_grouped_parameters = [
        {
            "params": [p for n, p in model.named_parameters() if not any(nd in n for nd in no_decay)],
            "weight_decay": args.weight_decay,
        },
        {
            "params": [p for n, p in model.named_parameters() if any(nd in n for nd in no_decay)],
            "weight_decay": 0.0,
        },
    ]
    optimizer = torch.optim.AdamW(optimizer_grouped_parameters, lr=args.lr)

    num_update_steps_per_epoch = math.ceil(len(train_loader) / args.accumulate_step)
    max_train_steps = args.num_epoch * num_update_steps_per_epoch

    # learning_rate scheduler
    lr_scheduler = get_scheduler(
        name=args.lr_scheduler_type,
        optimizer=optimizer,
        num_warmup_steps=args.num_warmup_steps,
        num_training_steps=max_train_steps
    )

    model, optimizer, train_loader, valid_loader, lr_scheduler = accelerator.prepare(
        model, optimizer, train_loader, valid_loader, lr_scheduler
    )
    
    # Metrics
    metric = load_metric("accuracy")

    # Start Training!
    total_batch_size = args.per_device_train_batch_size * accelerator.num_processes * args.accumulate_step

    logger.info("***** Running training *****")
    logger.info(f"  Num examples = {len(train_dataset)}")
    logger.info(f"  Num Epochs = {args.num_epoch}")
    logger.info(f"  Instantaneous batch size per device = {args.per_device_train_batch_size}")
    logger.info(f"  Total train batch size (w. parallel, distributed & accumulation) = {total_batch_size}")
    logger.info(f"  Gradient Accumulation steps = {args.accumulate_step}")
    logger.info(f"  Total optimization steps = {max_train_steps}")

    progress_bar = tqdm(range(max_train_steps), disable=not accelerator.is_local_main_process)
    completed_steps = 0

    for epoch in range(args.num_epoch):
        model.train()
        optimizer.zero_grad()

        for step, batch in enumerate(train_loader):
            outputs = model(**batch)
            loss = outputs.loss
            loss = loss / args.accumulate_step

            accelerator.backward(loss)
            if step % args.accumulate_step == 0 or step == len(train_loader) - 1:
                optimizer.step()
                lr_scheduler.step()
                optimizer.zero_grad()

                progress_bar.set_postfix({"loss": f"{loss.item():.3f}"})
                progress_bar.update(1)
                completed_steps += 1

            if completed_steps >= max_train_steps:
                break
        
        # Evaluation
        model.eval()
        samples_seen = 0
        for batch in valid_loader:   
            with torch.no_grad():
                outputs = model(**batch)  

            predictions = outputs.logits.argmax(dim=-1)
            print(predictions, batch["labels"])
            predictions, references = accelerator.gather_for_metrics((predictions, batch["labels"]))

            if accelerator.num_processes > 1:
                if step == len(valid_loader):
                    predictions = predictions[: len(valid_loader.dataset) - samples_seen]
                    references = references[: len(valid_loader.dataset) - samples_seen]
                else:
                    samples_seen += references.shape[0]
                    
            metric.add_batch(
                predictions=predictions,
                references=references,
            )
            
        eval_metric = metric.compute()
        accelerator.print(f"epoch {epoch}: {eval_metric}")

        accelerator.wait_for_everyone()
        unwrapped_model = accelerator.unwrap_model(model)
        unwrapped_model.save_pretrained(
            args.model_save_dir, is_main_process=accelerator.is_main_process, save_function=accelerator.save
        )

if __name__ == "__main__":
    args = parse_args()
    args.model_save_dir.mkdir(parents=True, exist_ok=True)
    main(args)