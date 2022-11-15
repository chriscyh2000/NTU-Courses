import json
from argparse import ArgumentParser, Namespace
from pathlib import Path

def main(args):
    train_file = args.data_dir / "train.json"
    train_data = json.loads(train_file.read_text())
    eval_file = args.data_dir / "eval.json"
    eval_data = json.loads(eval_file.read_text())
    
    processed_train_data = []
    for instance in train_data:
        processed_train_data.append({
            "id": instance["id"],
            "tokens": instance["tokens"],
            "ner_tags": instance["tags"]
        })
    processed_eval_data = []
    for instance in eval_data:
        processed_eval_data.append({
            "id": instance["id"],
            "tokens": instance["tokens"],
            "ner_tags": instance["tags"]
        })

    processed_train_file = args.output_dir / "train.json"
    processed_train_file.write_text(json.dumps(processed_train_data, indent=2))
    processed_eval_file = args.output_dir / "eval.json"
    processed_eval_file.write_text(json.dumps(processed_eval_data, indent=2))
    

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--data_dir",
        type=Path,
        help="Directory to the dataset.",
        default="../data/slot/",
    )
    parser.add_argument(
        "--output_dir",
        type=Path,
        help="Directory to save the processed file.",
        default="./processed_data/slot/",
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    main(args)
