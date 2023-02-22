import random
import math
import numpy as np

from pathlib import Path
from argparse import ArgumentParser, Namespace
from tqdm import tqdm

from dataset import loadDatasets
from preprocess import preprocess

import torch
from torch.utils.data import DataLoader
from transformers import AutoTokenizer, AutoModelForSeq2SeqLM, DataCollatorForSeq2Seq
from transformers.optimization import Adafactor, AdafactorSchedule

import logging

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--data_path",
        type=Path,
        help="Path to the data",
        default="./data/train.jsonl"
    )
    parser.add_argument(
        "--pretrained_model",
        type=str,
        help="Pretrained model hugging face repo name or path to local directory",
        default="google/mt5-base"
    )
    parser.add_argument(
        "--model_save_dir",
        type=Path,
        help="Path to save trained model",
        default="./ckpt/trained"
    )

    # optimizer
    parser.add_argument(
        "--lr",
        type=float,
        help="learning rate",
        default=None
    )
    parser.add_argument(
        "--weight_decay",
        type=float,
        default=0.0
    )

    parser.add_argument(
        "--num_epoch",
        type=int,
        help="number of epoch",
        default=4
    )
    parser.add_argument(
        "--batch_size",
        type=int,
        help="Batch size",
        default=1
    )
    parser.add_argument(
        "--gradient_accumulation_steps",
        type=int,
        help="number of gradient accumulation steps",
        default=8
    )

    parser.add_argument(
        "--max_input_length",
        type=int,
        help="maximum length of the input article",
        default=256
    )
    parser.add_argument(
        "--max_target_length",
        type=int,
        help="maximum length of the summary",
        default=64
    )

    parser.add_argument(
        "--device",
        type=torch.device, 
        help="cpu, cuda, cuda:0, cuda:1", 
        default="cuda:0"
    )

    parser.add_argument(
        "--no_train",
        action="store_true",
        help="If you don't want to train the model, add this option.",
    )
    args = parser.parse_args()
    return args

def main(args):
    logging.basicConfig(
        format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
        datefmt="%m/%d/%Y %H:%M:%S",
        level=logging.INFO,
    )

    datasets = loadDatasets(args.data_path, test_size=0.1, split="train", other_split="val")
    tokenizer = AutoTokenizer.from_pretrained(args.pretrained_model)
    
    tokenized_dataset = datasets.map(
        lambda examples: preprocess(
            tokenizer, 
            examples,
            "train",
            args.max_input_length, 
            args.max_target_length,
        ),
        batched=True,
        remove_columns=datasets["train"].column_names,
        desc="Running tokenizer on dataset"
    )
    train_dataset = tokenized_dataset["train"]
    eval_dataset = tokenized_dataset["val"]

    for index in random.sample(range(len(train_dataset)), 1):
        logging.info(f"Sample {index} of the training set: {train_dataset[index]}.")

    print(f"GPU is available: {torch.cuda.is_available()}")
    model = AutoModelForSeq2SeqLM.from_pretrained(
        args.model_save_dir if args.no_train else args.pretrained_model
    ).to(args.device)
    
    data_collator = DataCollatorForSeq2Seq(
        tokenizer, 
        model=model, 
        return_tensors="pt",
        label_pad_token_id=-100
    )

    train_dataloader = DataLoader(
        train_dataset,
        shuffle=True,
        collate_fn=data_collator,
        batch_size=args.batch_size
    )
    eval_dataloader = DataLoader(
        eval_dataset,
        shuffle=False,
        collate_fn=data_collator,
        batch_size=args.batch_size
    )
    
    optimizer = Adafactor(model.parameters(), scale_parameter=True, relative_step=True, warmup_init=True, lr=args.lr)
    scheduler = AdafactorSchedule(optimizer)

    logging.info("***** Run training *****")
    logging.info(f"  Num Examples = {len(train_dataset)}")
    logging.info(f"  Num Epochs = {args.num_epoch}")
    logging.info(f"  Batch Size = {args.batch_size}")
    logging.info(f"  Gradient Accumulation steps = {args.gradient_accumulation_steps}")


    best_loss = math.inf
    for epoch in range(args.num_epoch):
        train_pbar = tqdm(train_dataloader, position=0, leave=True, desc=f"Epoch: [{epoch + 1} / {args.num_epoch}] | Training")

        accumulated_loss = 0.0
        model.train()
        for step, batch in enumerate(train_pbar):
            if args.no_train: break
            batch = {key: val.to(args.device) for key, val in batch.items()}
            outputs = model(**batch)
            loss = outputs.loss
            loss.backward()

            if (step + 1) % args.gradient_accumulation_steps == 0 or (step + 1) == len(train_dataloader):
                optimizer.step()
                scheduler.step()
                optimizer.zero_grad()
            
            accumulated_loss += loss.item()
            train_pbar.set_postfix({"loss": f"{accumulated_loss / (step + 1):.3f}"})
        
        eval_pbar = tqdm(eval_dataloader, position=0, leave=True, desc=f"Epoch: [{epoch + 1} / {args.num_epoch}] | Evaluating")
        accumulated_loss = 0.0
        model.eval()
        for batch in eval_pbar:
            batch = {key: val.to(args.device) for key, val in batch.items()}
            outputs = model(**batch)
            loss = outputs.loss
            accumulated_loss += loss.item()

        if accumulated_loss / len(eval_pbar) < best_loss:
            best_loss = accumulated_loss / len(eval_pbar)
            print(f"Saving model... best_loss = {best_loss:.3f}")
            model.save_pretrained(args.model_save_dir)


if __name__ == "__main__":
    args = parse_args()
    args.model_save_dir.mkdir(exist_ok=True, parents=True)
    main(args)
