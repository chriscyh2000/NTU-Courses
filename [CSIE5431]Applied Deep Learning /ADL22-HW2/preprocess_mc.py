import json
import logging
from argparse import ArgumentParser, Namespace
from pathlib import Path
from random import seed
from transformers import AutoTokenizer

logging.basicConfig(
    format="%(asctime)s | %(levelname)s | %(message)s",
    level=logging.INFO,
    datefmt="%Y-%m-%d %H:%M:%S",
)

def preprocess_function(dataset, context, tokenizer, max_length):
    question = [[instance["question"]] * 4 for instance in dataset]
    paragraphs = [[context[idx] for idx in instance["paragraphs"]] for instance in dataset]

    question = sum(question, [])
    paragraphs = sum(paragraphs, [])
    tokenized_input = tokenizer(
        question, 
        paragraphs, 
        max_length=max_length, 
        padding="max_length",
        truncation=True, 
    )

    tokenized_input = {key: [val[i:i+4] for i in range(0, len(question), 4)] for key, val in tokenized_input.items()}

    if dataset and type(dataset[0]) is dict:
        if "relevant" in dataset[0]: 
            tokenized_input["labels"] = [instance["paragraphs"].index(instance["relevant"]) for instance in dataset]
        else:
            tokenized_input["labels"] = [0 for _ in range(len(dataset))]

    return tokenized_input


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--train_file",
        type=Path,
        help="Directory to the train.json .",
        default=None,
    )
    parser.add_argument(
        "--valid_file",
        type=Path,
        help="Directory to the valid.json .",
        default=None,
    )
    parser.add_argument(
        "--test_file",
        type=Path,
        help="Directory to the test.json .",
        default=None,
    )
    parser.add_argument(
        "--context_file",
        type=Path,
        help="Directory to the context.json .",
        default="./data/context.json",
    )
    parser.add_argument("--rand_seed", type=int, help="Random seed.", default=13)
    parser.add_argument(
        "--tokenizer_name_or_path",
        type=Path,
        help="Directory to the pretrained tokenizer.",
        default="./ckpt/pretrained/roberta/",
    )
    parser.add_argument(
        "--max_length",
        type=int,
        help="Maximum length to truncate",
        default=512,
    )
    parser.add_argument(
        "--processed_data_dir",
        type=Path,
        help="Directory to save the processed file.",
        default="./processed_data/mc/",
    )

    args = parser.parse_args()
    return args

def main(args):
    seed(args.rand_seed)

    tokenizer = AutoTokenizer.from_pretrained(args.tokenizer_name_or_path, use_fast=True)
    context = json.loads(args.context_file.read_text())

    files = {
        "train": args.train_file, 
        "valid": args.valid_file,
        "test": args.test_file
    }
    for split in ["train", "valid", "test"]:
        data_path = files[split]
        if data_path == None: continue

        dataset = json.loads(data_path.read_text())
        features = preprocess_function(dataset, context, tokenizer, args.max_length)

        # print("sample:", [tokenizer.decode(features["input_ids"][0][i]) for i in range(4)])

        token_path = args.processed_data_dir / (split + "_token.json")
        with open(token_path, "w") as f:
            f.write(json.dumps(features))

        logging.info(f"{split.capitalize()} token saved at {str(token_path.resolve())}")

if __name__ == "__main__":
    args = parse_args()
    args.processed_data_dir.mkdir(parents=True, exist_ok=True)
    main(args)