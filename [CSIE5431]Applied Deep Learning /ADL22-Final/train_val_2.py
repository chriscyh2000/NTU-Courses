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
        "--val_user_data",
        type=Path,
        default="processed_datas/val_unseen_users.json"
    )
    parser.add_argument(
        "--course_data_path",
        type=Path,
        default="processed_datas/train_courses.json"
    )
    parser.add_argument(
        "--train_users_embedding",
        type=Path,
        default="./processed_datas/train_users_text2vec.pkl"
    )
    parser.add_argument(
        "--train_courses_embedding",
        type=Path,
        default="./processed_datas/train_courses_text2vec.pkl"
    )
    parser.add_argument(
        "--val_users_embedding",
        type=Path,
        default="./processed_datas/val_unseen_users_text2vec.pkl"
    )
    # for training
    parser.add_argument(
        "--similar_course_data",
        type=Path,
        default="./unseen_similar_courses.json"
    )
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
    parser.add_argument(
        "--add_unseen_courses",
        action="store_true",
        help="If you want to add unseen courses",
    )
    # output path
    parser.add_argument(
        "--ckpt_dir",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpts/attn_val"
    )
    parser.add_argument(
        "--output_result",
        action="store_true",
        help="If you want the decoder to output result",
    )
    parser.add_argument(
        "--users_seen",
        action="store_true",
        help="If you want to train the model for the seen users",
    )
    args = parser.parse_args()
    return args

def preprocess(example, unseen_similar_course, add_unseen_courses=True):
    for i, data in enumerate(example["labels"]):
        new_label = [label - 4 for label in data]
        example["labels"][i] = new_label
    
    unseen_labels = example.get("unseen_labels", None)
    if unseen_labels is not None:
        for i, data in enumerate(example["unseen_labels"]):
            new_label = [label - 4 for label in data]
            example["unseen_labels"][i] = new_label
    
    if add_unseen_courses:
        for i, data in enumerate(example["labels"]):
            added_course = random.choices(
                list(set([random.choice(unseen_similar_course[id]) for id in data])),
                k=random.choice([0, 1, 2, 3, 4, 5, 6])
            )
            example["labels"][i].extend(added_course)
            
    return example

def collate_fn(examples):    
    new_labels = torch.zeros(len(examples), 728)
    for i, example in enumerate(examples):
        new_labels[i][example["labels"]] = 1
        
    if examples[0].get("unseen_labels", None) is not None:
        unseen_labels = [example["unseen_labels"] for example in examples]
    else:
        unseen_labels = None
        
    batch = {
        "input_ids": torch.FloatTensor([example["input_ids"] for example in examples]),
        "labels": new_labels,
        "unseen_labels": unseen_labels
    }
    return batch
    
def main(args):
    # data processing
    with open(args.train_users_embedding, "rb") as f:
        train_users_embedding: List = pickle.load(f)
    with open(args.train_courses_embedding, "rb") as f:
        courses_embedding: List = pickle.load(f)
    with open(args.val_users_embedding, "rb") as f:
        val_users_embedding: List = pickle.load(f)
            
    unseen_similar_course = json.loads(args.similar_course_data.read_text())
    
    # training set
    user_info = json.loads(args.train_user_data.read_text())
    labels = [user["labels"] for user in user_info]
    
    train_data_dict = {
        "input_ids": train_users_embedding,
        "labels": labels,
    }
    train_dataset = Dataset.from_dict(train_data_dict)
    train_dataset  = train_dataset.map(
        lambda example: preprocess(
            example, 
            unseen_similar_course, 
            args.add_unseen_courses
        ),
        batched=True,
        desc="training_set: labels minus 4" + " and add unseen courses" if args.add_unseen_courses else ""
    )
    
    # validation set
    eval_user_info = json.loads(args.val_user_data.read_text())
    if args.users_seen:
        user_dict = {user["id"]: user["labels"] for user in user_info}
        labels = [user_dict[user["id"]] for user in eval_user_info]
        unseen_labels = [user["labels"] for user in eval_user_info]
    else:
        labels = [user["labels"] for user in user_info]
        unseen_labels = None
        
    eval_data_dict = {
        "input_ids": val_users_embedding,
        "labels": labels,
    }
    if unseen_labels is not None:
        eval_data_dict.update({"unseen_labels": unseen_labels})
    
    eval_dataset = Dataset.from_dict(eval_data_dict)
    eval_dataset  = eval_dataset.map(
        lambda example: preprocess(
            example, 
            None, 
            False
        ),
        batched=True,
        desc="eval_set: labels minus 4"
    )
    
    courses_embedding = torch.FloatTensor(courses_embedding)

    # training
    model = Attn_valuebase(hidden_dim=768, query_dim=512, key_dim=512).to(args.device)
    optimizer = Adafactor(model.parameters(), scale_parameter=True, relative_step=True, warmup_init=True, lr=args.lr)
    scheduler = AdafactorSchedule(optimizer)
    
    train_dataloader = DataLoader(
        train_dataset,
        shuffle=True,
        collate_fn=lambda examples: collate_fn(
            examples,
            # unseen_similar_course, 
            # args.add_unseen_courses
        ),
        batch_size=args.batch_size
    )
    eval_dataloader = DataLoader(
        eval_dataset,
        shuffle=False,
        collate_fn=lambda examples: collate_fn(
            examples,
            # None, 
            # False
        ),
        batch_size=1
    )
    
    epoch_without_update = 0
    best_score = -math.inf
    for epoch in range(args.num_epoch):
        train_pbar = tqdm(train_dataloader, position=0, leave=True, desc=f"Epoch: [{epoch + 1} / {args.num_epoch}] | Training")
        accumulated_loss = 0.0
        score = 0.0
        model.train()
        
        for step, batch in enumerate(train_pbar):
            batch["course_input_ids"] = courses_embedding
            batch["user_input_ids"] = batch.pop("input_ids")
            batch = {
                key: (val.to(args.device) if str(type(val)) == "<class 'torch.Tensor'>" else val) for key, val in batch.items()
            }
            
            if args.output_result: _, loss, result, batch_score = model(args.output_result, **batch)
            else: _, loss, batch_score = model(args.output_result, **batch)
            
            loss.backward()
            
            optimizer.step()
            scheduler.step()
            optimizer.zero_grad()
            
            accumulated_loss += loss.item()
                
            if args.output_result:
                print(labels)
                print(result.tolist())
            score += batch_score
            train_pbar.set_postfix({"loss": f"{accumulated_loss / (step + 1):.3f}", "score": f"{score / (step + 1):.3f}"})
            
        
        eval_pbar = tqdm(eval_dataloader, position=0, leave=True, desc=f"Epoch: [{epoch + 1} / {args.num_epoch}] | Evaluating")
        accumulated_loss = 0.0
        score = 0.0
        model.eval()
        
        for batch in eval_pbar:
            with torch.no_grad():
                batch["course_input_ids"] = courses_embedding
                batch["user_input_ids"] = batch.pop("input_ids")
                batch = {
                    key: (val.to(args.device) if str(type(val)) == "<class 'torch.Tensor'>" else val) for key, val in batch.items()
                }
                
                if args.output_result:_, loss, result, batch_score = model(args.output_result, **batch)
                else: _, loss, batch_score = model(args.output_result, args.users_seen, **batch)
                
                accumulated_loss += loss.item()
                labels = []
                for label in batch["labels"]:
                    labels.append(label.nonzero().squeeze(-1).tolist())
                
                if args.output_result:
                    print(labels)
                    print(result.tolist())
                score += batch_score

        print(f"score: {(score / len(eval_pbar)):.3f}")

        if (score / len(eval_pbar)) > best_score:
            best_score = (score / len(eval_pbar))
            print(f"Saving model... best_score = {best_score:.3f}")
            torch.save(model.state_dict(), args.ckpt_dir / "model.ckpt")
            epoch_without_update = 0
        else: 
            epoch_without_update += 1
            
        if epoch_without_update >= args.early_stop_epochs:
            break
    
if __name__ == "__main__":
    args = parse_args()
    args.ckpt_dir.mkdir(parents=True, exist_ok=True)
    main(args)
    
