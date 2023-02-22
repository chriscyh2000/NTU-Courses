from encoder import BaseEncoder
from argparse import ArgumentParser, Namespace
from transformers import AutoTokenizer, DataCollatorForSeq2Seq
from models import seq2seqModel_MeanPoolingAttn
from transformers.optimization import Adafactor, AdafactorSchedule
import torch
from torch.utils.data import DataLoader

from datasets import Dataset, DatasetDict
import json
import pickle
from pathlib import Path
from typing import List

import random
import math

from tqdm import tqdm
import logging

logging.basicConfig(
    format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
    datefmt="%m/%d/%Y %H:%M:%S",
    level=logging.INFO,
)


def parse_args() -> Namespace:
    parser = ArgumentParser()
    # input data
    parser.add_argument(
        "--user_data_path",
        type=Path,
        default="processed_datas/train_users.json"
    )
    parser.add_argument(
        "--course_data_path",
        type=Path,
        default="processed_datas/train_courses.json"
    )
    # encoder and tokenizer
    parser.add_argument(
        "--encoder_name",
        type=str,
        default='google/mt5-small'
    )
    # for tokenizer
    parser.add_argument(
        "--seq_maxlen",
        type=int,
        default=128
    )
    # for training
    parser.add_argument(
        "--device",
        type=torch.device,
        default="cuda:0"
    )
    parser.add_argument(
        "--test_size",
        type=float,
        default=0.1
    )
    parser.add_argument(
        "--num_epoch",
        type=int,
        default=50
    )
    parser.add_argument(
        "--batch_size",
        type=int,
        default=8
    )
    parser.add_argument(
        "--early_stop_epochs",
        type=int,
        default=120
    )
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
    # output path
    parser.add_argument(
        "--ckpt_dir",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpt"
    )
    args = parser.parse_args()
    return args

def preprocess(example, max_len):
    for data in example["labels"]:
        while len(data) < max_len:
            data.append(-100)
    return example

def collate_fn(examples):
    batch = {
        "input_ids": torch.FloatTensor([example["input_ids"] for example in examples]),
        "labels": torch.LongTensor([example["labels"] for example in examples])
    }
    return batch
    
def main(args):
    tokenizer = AutoTokenizer.from_pretrained(args.encoder_name)
    # data processing
    with open("./processed_datas/train_users_embeddings.pkl", "rb") as f:
        users_embedding: List = pickle.load(f)
    with open("./processed_datas/train_courses_embeddings.pkl", "rb") as f:
        courses_embedding: List = pickle.load(f)
        
    user_info = json.loads(args.user_data_path.read_text())
    labels = [user["labels"] for user in user_info]
    max_label_len = max([len(label) for label in labels])
    
    data_dict = {
        "input_ids": users_embedding,
        "labels": labels
    }
    dataset = Dataset.from_dict(data_dict)
    dataset_dict = dataset.train_test_split(args.test_size)
    datasets = DatasetDict(
        {"train": dataset_dict["train"], "eval": dataset_dict["test"]}
    )
    datasets = datasets.map(
        lambda example: preprocess(example, max_label_len),
        batched=True,
        desc="convert input data into tensor"
    )
    train_dataset = datasets["train"]
    eval_dataset = datasets["eval"]
    courses_embedding = torch.FloatTensor(courses_embedding)
    
    # for index in random.sample(range(len(train_dataset)), 1):
    #     logging.info(f"Sample {index} of the training set: {train_dataset[index]}.")
    # for index in random.sample(range(len(eval_dataset)), 1):
    #     logging.info(f"Sample {index} of the evaluating set: {eval_dataset[index]}.")

    # training
    model = seq2seqModel_MeanPoolingAttn(args.encoder_name).to(args.device)
    optimizer = Adafactor(model.parameters(), scale_parameter=True, relative_step=True, warmup_init=True, lr=args.lr)
    scheduler = AdafactorSchedule(optimizer)
    
    train_dataloader = DataLoader(
        train_dataset,
        shuffle=True,
        collate_fn=collate_fn,
        batch_size=args.batch_size
    )
    eval_dataloader = DataLoader(
        eval_dataset,
        shuffle=False,
        collate_fn=collate_fn,
        batch_size=args.batch_size
    )
    
    epoch_without_update = 0
    best_loss = math.inf
    for epoch in range(args.num_epoch):
        train_pbar = tqdm(train_dataloader, position=0, leave=True, desc=f"Epoch: [{epoch + 1} / {args.num_epoch}] | Training")
        accumulated_loss = 0.0
        model.train()
        
        for step, batch in enumerate(train_pbar):
            batch["course_input_ids"] = courses_embedding
            batch["user_input_ids"] = batch.pop("input_ids")
            batch = {key: val.to(args.device) for key, val in batch.items()}
            outputs, loss = model(**batch)
            loss.backward()
            
            optimizer.step()
            scheduler.step()
            optimizer.zero_grad()
            
            accumulated_loss += loss.item()
            train_pbar.set_postfix({"loss": f"{accumulated_loss / (step + 1):.3f}"})
        
        eval_pbar = tqdm(eval_dataloader, position=0, leave=True, desc=f"Epoch: [{epoch + 1} / {args.num_epoch}] | Evaluating")
        accumulated_loss = 0.0
        model.eval()
        
        for batch in eval_pbar:
            batch["course_input_ids"] = courses_embedding
            batch["user_input_ids"] = batch.pop("input_ids")
            batch = {key: val.to(args.device) for key, val in batch.items()}
            outputs, loss = model(**batch)
            accumulated_loss += loss.item()

        if accumulated_loss / len(eval_pbar) < best_loss:
            best_loss = accumulated_loss / len(eval_pbar)
            print(f"Saving model... best_loss = {best_loss:.3f}")
            model.save_pretrained(args.ckpt_dir)
            epoch_without_update += 1
        else: 
            epoch_without_update = 0
            
        if epoch_without_update >= args.early_stop_epochs:
            break
    
if __name__ == "__main__":
    args = parse_args()
    args.ckpt_dir.mkdir(parents=True, exist_ok=True)
    main(args)
    