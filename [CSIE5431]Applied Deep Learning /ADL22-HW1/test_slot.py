import json
import pickle
from argparse import ArgumentParser, Namespace
from pathlib import Path
from typing import Dict

import torch
from torch.utils.data import DataLoader
from tqdm import tqdm

from dataset import SeqTaggingClsDataset
from model import SeqTagger
from utils import Vocab


def main(args):
    # TODO: implement main function
    with open(args.cache_dir / "vocab.pkl", "rb") as f:
        vocab: Vocab = pickle.load(f)

    tag_idx_path = args.cache_dir / "tag2idx.json"
    tag2idx: Dict[str, int] = json.loads(tag_idx_path.read_text())

    data = json.loads(args.test_file.read_text())
    dataset = SeqTaggingClsDataset(data, vocab, tag2idx, args.max_len)

    embeddings = torch.load(args.cache_dir / "embeddings.pt")

    model = SeqTagger(
        embeddings=embeddings,
        num_layers=args.num_layers,
        hidden_size=args.hidden_size,
        bidirectional=args.bidirectional,
        num_class=len(list(tag2idx.keys())),
        dropout=args.dropout,
        device=args.device
    ).to(args.device)

    ckpt = torch.load(args.ckpt_path)
    model.load_state_dict(ckpt)

    # TODO: predict dataset
    testing_loader = DataLoader(
        dataset, 
        batch_size=args.batch_size, 
        shuffle=False, 
        collate_fn=dataset.collate_fn,
        pin_memory=True
    )

    # TODO: write prediction to file (args.pred_file)
    test_pbar = tqdm(testing_loader, position=0, leave=True, desc=f"Testing: ")

    ans = []
    for data in test_pbar:
        with torch.no_grad():
            pred_ = model(data)

            for id, probs in pred_.items():
                probs = torch.softmax(probs.cpu(), dim=-1)
                probs = torch.argmax(probs, dim=-1)
                ans.append((id, [dataset.idx2tag(prob.item()) for prob in probs]))


    print("PREDICTION COMPLETED !!!")

    # write the prediction results into pred.csv
    with open(args.pred_file, "w+") as f:
        f.write("id,tags\n")
        for id, tags in ans:
            f.write(f"{id},{' '.join(tags)}\n")

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--test_file",
        type=Path,
        help="Path to the test file.",
        required=True
    )
    parser.add_argument(
        "--cache_dir",
        type=Path,
        help="Directory to the preprocessed caches.",
        default="./cache/slot/",
    )
    parser.add_argument(
        "--ckpt_path",
        type=Path,
        help="Path to model checkpoint.",
        required=True,
    )
    parser.add_argument("--pred_file", type=Path, default="./pred/slot_pred")

    # data
    parser.add_argument("--max_len", type=int, default=35)

    # model
    parser.add_argument("--hidden_size", type=int, default=128)
    parser.add_argument("--num_layers", type=int, default=2)
    parser.add_argument("--dropout", type=float, default=0.1)
    parser.add_argument("--bidirectional", type=bool, default=True)

    # data loader
    parser.add_argument("--batch_size", type=int, default=128)

    parser.add_argument(
        "--device", type=torch.device, help="cpu, cuda, cuda:0, cuda:1", default="cuda:0"
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    args.pred_file.parent.resolve().mkdir(parents=True, exist_ok=True)
    main(args)