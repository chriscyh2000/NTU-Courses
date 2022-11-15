import json
import pickle
from argparse import ArgumentParser, Namespace
from pathlib import Path
from typing import Dict

import torch

from torch.utils.data import DataLoader
from dataset import SeqClsDataset
from model import SeqClassifier
from utils import Vocab
from tqdm import tqdm


def main(args):
    with open(args.cache_dir / "vocab.pkl", "rb") as f:
        vocab: Vocab = pickle.load(f)

    intent_idx_path = args.cache_dir / "intent2idx.json"
    intent2idx: Dict[str, int] = json.loads(intent_idx_path.read_text())

    data = json.loads(args.test_file.read_text())
    dataset = SeqClsDataset(data, vocab, intent2idx, args.max_len)
    # TODO: crecate DataLoader for test dataset

    embeddings = torch.load(args.cache_dir / "embeddings.pt")

    model = SeqClassifier(
        embeddings=embeddings,
        num_layers=args.num_layers,
        bidirectional=args.bidirectional,
        num_class=len(list(intent2idx.keys())),
        dropout=args.dropout,
        device=args.device
    ).to(args.device)
    model.eval()

    ckpt = torch.load(args.ckpt_path)
    model.load_state_dict(ckpt) # load weights into model

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

            pred = torch.Tensor([]).to(args.device)
            for probs in pred_.values():
                pred = torch.cat((pred, probs.view(1, -1)))
            
            pred = torch.argmax(pred.cpu(), dim=-1)
            batch_ans = [dataset.idx2label(int(i.item())) for i in pred]
            ans.extend(zip(data.keys(), batch_ans))


    print("PREDICTION COMPLETED !!!")

    # write the prediction results into pred.csv
    with open(args.pred_file, "w+") as f:
        f.write("id,intent\n")
        for id, cls in ans:
            f.write(f"{id},{cls}\n")

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
        default="./cache/intent/",
    )
    parser.add_argument(
        "--ckpt_path",
        type=Path,
        help="Path to model checkpoint.",
        required=True,
    )
    parser.add_argument("--pred_file", type=Path, default="./pred/intent_pred.csv")

    # data
    parser.add_argument("--max_len", type=int, default=28)

    # model
    parser.add_argument("--hidden_size", type=int, default=512)
    parser.add_argument("--num_layers", type=int, default=3)
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
