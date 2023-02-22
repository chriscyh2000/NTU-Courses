from encoder import BaseEncoder
from argparse import ArgumentParser, Namespace
from transformers import AutoTokenizer, DataCollatorForSeq2Seq
from models import Attn_valuebase
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

from utils import mapk

logging.basicConfig(
    format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
    datefmt="%m/%d/%Y %H:%M:%S",
    level=logging.INFO,
)


def parse_args() -> Namespace:
    parser = ArgumentParser()
    # input data
    parser.add_argument(
        "--train_user_data",
        type=Path,
        default="processed_datas/train_users.json"
    )
    parser.add_argument(
        "--test_user_data",
        type=Path,
        default="processed_datas/test_unseen_users.json"
    )
    parser.add_argument(
        "--course_data_path",
        type=Path,
        default="processed_datas/train_courses.json"
    )
    parser.add_argument(
        "--train_courses_embedding",
        type=Path,
        default="./processed_datas/train_courses_text2vec.pkl"
    )
    parser.add_argument(
        "--test_users_embedding",
        type=Path,
        default="./processed_datas/test_unseen_users_text2vec.pkl"
    )
    # for training
    parser.add_argument(
        "--device",
        type=torch.device,
        default="cuda:0"
    )
    # output path
    parser.add_argument(
        "--ckpt_path",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpt/attn_val/model.ckpt"
    )
    parser.add_argument(
        "--pred_file",
        type=Path,
        default="./pred/test_unseen_courses.csv"
    )
    parser.add_argument(
        "--users_seen",
        action="store_true",
        help="If you want to train the model for the seen users",
    )
    args = parser.parse_args()
    return args

def preprocess(example):
    if example.get("labels", None) is not None:
        for i, data in enumerate(example["labels"]):
            new_label = [label - 4 for label in data]
            example["labels"][i] = new_label
    return example

def collate_fn(examples):
    if examples[0].get("labels", None) is not None:
        new_labels = torch.zeros(len(examples), 728)
        for i, example in enumerate(examples):
            new_labels[i][example["labels"]] = 1
    else:
        new_labels = None
    batch = {
        "input_ids": torch.FloatTensor([example["input_ids"] for example in examples]),
        "labels": new_labels,
    }
    return batch

def main(args):
    # data processing
    with open("./course_tokenizer.json") as f:
        tokenized_course = json.load(f)
    with open(args.train_courses_embedding, "rb") as f:
        courses_embedding: List = pickle.load(f)
    with open(args.test_users_embedding, "rb") as f:
        test_users_embedding: List = pickle.load(f)
            
    tokenized_course = tokenized_course["added_tokens"]
    
    test_user_info = json.loads(args.test_user_data.read_text())
    
    if args.users_seen:
        train_user_info = json.loads(args.train_user_data.read_text())
        user_dict = {user["id"]: user["labels"] for user in train_user_info}
        labels = [user_dict[user["id"]] for user in test_user_info]
    else:
        labels = None
    
    test_data_dict = {
        "input_ids": test_users_embedding,
    }
    if labels is not None:
        test_data_dict.update({"labels": labels})
    test_dataset = Dataset.from_dict(test_data_dict)
    test_dataset  = test_dataset.map(
        preprocess,
        batched=True,
        desc="testing_set: labels minus 4" if args.users_seen else ""
    )
    
    courses_embedding = torch.FloatTensor(courses_embedding)
    
    # training
    model = Attn_valuebase(hidden_dim=768, query_dim=512, key_dim=512).to(args.device)
    ckpt = torch.load(args.ckpt_path)
    model.load_state_dict(ckpt)
    
    test_dataloader = DataLoader(
        test_dataset,
        shuffle=False,
        collate_fn=collate_fn,
        batch_size=1
    )
    
    results = []
    test_pbar = tqdm(test_dataloader, position=0, leave=True, desc=f"Testing: ")
    model.eval()
    for batch in test_pbar:
        with torch.no_grad():
            batch["course_input_ids"] = courses_embedding
            batch["user_input_ids"] = batch.pop("input_ids")
            batch = {
                key: (val.to(args.device) if str(type(val)) == "<class 'torch.Tensor'>" else val) for key, val in batch.items()
            }
            _, _, result = model(True, args.users_seen, **batch)
            results.extend(result)
            
    results = [[tokenized_course[idx+4]["content"] for idx in result] for result in results]
    with open(args.pred_file, "w") as f:
        f.write("user_id,course_id\n")
        for user, result in zip(test_user_info, results):
            f.write(user["id"] + ",")
            for id in result:
                f.write(" " + id)
            f.write("\n")
    
if __name__ == "__main__":
    args = parse_args()
    args.pred_file.parent.resolve().mkdir(parents=True, exist_ok=True)
    main(args)
    