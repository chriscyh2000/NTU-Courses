import os
import json
from argparse import ArgumentParser, Namespace
from pathlib import Path
import math
import logging
from dataset import MC_Dataset

import torch
from torch.utils.data import DataLoader
from tqdm import tqdm

from transformers import (
    AutoModelForMultipleChoice,
    default_data_collator,
)
from accelerate import Accelerator

# os.environ["CUDA_VISIBLE_DEVICES"] = "5"

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
        "--processed_data_dir",
        type=Path,
        help="Directory to the preprocessed caches.",
        default="./processed_data/mc/",
    )
    parser.add_argument(
        "--model_save_dir",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpt/trained/mc/",
    )

    # data loader
    parser.add_argument(
        "--per_device_test_batch_size",
        type=int,
        default=1,
        help="Batch size (per device) for the testing dataloader.",
    )

    # pred
    parser.add_argument("--pred_file", type=Path, default="./pred/relevant_pred.json")

    args = parser.parse_args()
    return args

def main(args):
    accelerator = Accelerator()
    device = accelerator.device

    # Model
    model = AutoModelForMultipleChoice.from_pretrained(args.model_save_dir)
    model.to(device)

    # Dataset
    with accelerator.main_process_first(): 
        test_dataset_file = args.processed_data_dir / "test_token.json"
        test_dataset = MC_Dataset(test_dataset_file)

    # print(type(train_dataset))

    # DataLoader
    test_loader = DataLoader(
        test_dataset, shuffle=False, collate_fn=default_data_collator, batch_size=args.per_device_test_batch_size
    )


    model, test_loader, = accelerator.prepare(
        model, test_loader
    )

    # Start Training!

    logger.info("***** Running testing *****")
    logger.info(f"  Num examples = {len(test_dataset)}")
    logger.info(f"  Instantaneous batch size per device = {args.per_device_test_batch_size}")

    progress_bar = tqdm(range(len(test_loader)), disable=not accelerator.is_local_main_process)

    # Evaluation
    model.eval()
    samples_seen = 0
    final_predictions = []
    for step, batch in enumerate(test_loader):   
        with torch.no_grad():
            outputs = model(**batch)  

        predictions = outputs.logits.argmax(dim=-1)
        predictions, references = accelerator.gather_for_metrics((predictions, batch["labels"]))

        if accelerator.num_processes > 1:
            if step == len(test_loader):
                predictions = predictions[: len(test_loader.dataset) - samples_seen]
                references = references[: len(test_loader.dataset) - samples_seen]
            else:
                samples_seen += references.shape[0]
        
        final_predictions += [int(pred.item()) for pred in predictions]
        progress_bar.update(predictions.shape[0])

    with open(args.pred_file, "w+") as f:
        f.write(json.dumps(final_predictions))

if __name__ == "__main__":
    args = parse_args()
    args.pred_file.parent.resolve().mkdir(parents=True, exist_ok=True)
    main(args)