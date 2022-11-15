import json
from argparse import ArgumentParser, Namespace
from pathlib import Path

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
    parser.add_argument(
        "--processed_data_dir",
        type=Path,
        help="Directory to save the processed file.",
        default="./processed_data/",
    )
    parser.add_argument(
        "--relevant_pred_file",
        type=Path,
        help="Directory to the relevant prediction file.",
        default="./pred/relevant_pred.json",
    )

    args = parser.parse_args()
    return args

def main(args):
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

        processed_data_path = args.processed_data_dir / (split + ".json")
        data = dict()
        data["data"] = []
        if split == "test":
            relevant_pred = json.loads(args.relevant_pred_file.read_text())
            data["data"] = [{
                "context": context[instance["paragraphs"][relevant_pred[i]]], 
                "id": instance["id"], 
                "question": instance["question"]} for i, instance in enumerate(dataset)
            ] 

        else:
            data["data"] = [{
                "answers": {
                    "answer_start": [instance["answer"]["start"]],
                    "text": [instance["answer"]["text"]]
                }, 
                "context": context[instance["relevant"]], 
                "id": instance["id"], 
                "question": instance["question"]} for instance in dataset
            ] 

        with open(processed_data_path, "w", encoding="utf8") as f:
            f.write(json.dumps(data, ensure_ascii=False, indent=2))

if __name__ == "__main__":
    args = parse_args()
    args.processed_data_dir.mkdir(exist_ok=True, parents=True)
    main(args)