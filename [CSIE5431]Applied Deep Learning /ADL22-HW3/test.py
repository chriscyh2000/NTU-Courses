import random
import jsonlines

from pathlib import Path
from argparse import ArgumentParser, Namespace
from tqdm import tqdm

from dataset import loadDatasets
from preprocess import preprocess

import torch
from torch.utils.data import DataLoader
from transformers import AutoTokenizer, AutoModelForSeq2SeqLM, DataCollatorForSeq2Seq

import logging

import socket
def guard(*args, **kwargs):
    raise Exception("You are using the Internet!")
socket.socket = guard 

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--data_path",
        type=Path,
        help="Path to the data",
        default="./data/public.jsonl"
    )
    parser.add_argument(
        "--pretrained_model",
        type=str,
        help="Pretrained model hugging face repo name or path to local directory",
        default="./ckpt/trained"
    )
    parser.add_argument(
        "--model_save_dir",
        type=Path,
        help="Path to save trained model",
        default="./ckpt/trained"
    )
    parser.add_argument(
        "--pred_file",
        type=Path,
        help="Path to save prediction file",
        default="./pred/pred.jsonl"
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

    args = parser.parse_args()
    return args

def main(args):
    logging.basicConfig(
        format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
        datefmt="%m/%d/%Y %H:%M:%S",
        level=logging.INFO,
    )

    datasets = loadDatasets(args.data_path, test_size=0.0, split="test", other_split=None)
    tokenizer = AutoTokenizer.from_pretrained(args.pretrained_model)
    
    tokenized_dataset = datasets.map(
        lambda examples: preprocess(
            tokenizer, 
            examples,
            "test",
            args.max_input_length, 
            args.max_target_length,
        ),
        batched=True,
        remove_columns=datasets["test"].column_names,
        desc="Running tokenizer on dataset"
    )
    test_dataset = tokenized_dataset["test"]

    for index in random.sample(range(len(test_dataset)), 1):
        logging.info(f"Sample {index} of the testing set: {test_dataset[index]}.")

    print(f"GPU is available: {torch.cuda.is_available()}")
    model = AutoModelForSeq2SeqLM.from_pretrained(args.model_save_dir).to(args.device)
    data_collator = DataCollatorForSeq2Seq(
        tokenizer, 
        model=model, 
        return_tensors="pt",
        label_pad_token_id=-100
    )

    test_dataloader = DataLoader(
        test_dataset,
        shuffle=False,
        collate_fn=data_collator,
        batch_size=1
    )

    logging.info("***** Run testing *****")
    logging.info(f"  Num Examples = {len(test_dataset)}")

    test_pbar = tqdm(test_dataloader, position=0, leave=True, desc=f"Testing")
    model.eval()
    res = []
    for batch in test_pbar:
        batch = {key: torch.tensor(val).to(args.device) for key, val in batch.items()}

        generated_tokens = model.generate(batch["input_ids"], attention_mask=batch["attention_mask"], num_beams=7)
        if isinstance(generated_tokens, tuple):
            generated_tokens = generated_tokens[0]
        decoded_preds = tokenizer.batch_decode(
            generated_tokens, skip_special_tokens=True
        )
        res.extend(decoded_preds)

    with jsonlines.open(args.pred_file, mode="w") as writer:
        for i, pred in enumerate(res):
            writer.write({"title": pred, "id": datasets["test"][i]["id"]})
                
if __name__ == "__main__":
    args = parse_args()
    args.pred_file.parent.resolve().mkdir(parents=True, exist_ok=True)
    main(args)
