import json
import pickle
from argparse import ArgumentParser, Namespace
from pathlib import Path
from typing import Dict
import math


import torch
from torch.utils.data import DataLoader
from tqdm import tqdm
import numpy as np

from dataset import SeqClsDataset
from model import SeqClassifier
from utils import Vocab

TRAIN = "train"
DEV = "eval"
SPLITS = [TRAIN, DEV]

def main(args):
    with open(args.cache_dir / "vocab.pkl", "rb") as f:
        vocab: Vocab = pickle.load(f)

    intent_idx_path = args.cache_dir / "intent2idx.json"
    intent2idx: Dict[str, int] = json.loads(intent_idx_path.read_text())

    data_paths = {split: args.data_dir / f"{split}.json" for split in SPLITS}
    data = {split: json.loads(path.read_text()) for split, path in data_paths.items()}
    datasets: Dict[str, SeqClsDataset] = {
        split: SeqClsDataset(split_data, vocab, intent2idx, args.max_len)
        for split, split_data in data.items()
    }

    # TODO: crecate DataLoader for train / dev datasets
    training_dataset = datasets[TRAIN]
    evaluating_dataset = datasets[DEV]
    training_loader = DataLoader(
        training_dataset, 
        batch_size=args.batch_size, 
        shuffle=True, 
        collate_fn=training_dataset.collate_fn,
        pin_memory=True
    )
    evaluating_loader = DataLoader(
        evaluating_dataset, 
        batch_size=args.batch_size, 
        shuffle=True, 
        collate_fn=evaluating_dataset.collate_fn,
        pin_memory=True
    )

    embeddings = torch.load(args.cache_dir / "embeddings.pt")
    # TODO: init model and move model to target device(cpu / gpu)
    model = SeqClassifier(
        embeddings=embeddings,
        num_layers=args.num_layers,
        bidirectional=args.bidirectional,
        num_class=len(list(intent2idx.keys())),
        dropout=args.dropout,
        device=args.device
    ).to(args.device)

    # TODO: init optimizer
    optimizer = torch.optim.Adam(model.parameters(), lr=args.lr, weight_decay=2e-5)
    criterion = torch.nn.CrossEntropyLoss()

    best_loss = math.inf

    for epoch in range(args.num_epoch):
        # TODO: Training loop - iterate over train dataloader and update model weights
        # TODO: Evaluation loop - calculate accuracy and save model weights
        model.train()
        loss_record = []

        train_pbar = tqdm(training_loader, position=0, leave=True, desc=f"Epoch: [{epoch+1}/{args.num_epoch}]")
        for batch in train_pbar:
            optimizer.zero_grad()

            pred_ = model(batch)
            target = torch.LongTensor([label for _, label in batch.values()]).to(args.device)

            pred = torch.Tensor([]).to(args.device)
            for t in pred_.values():
                pred = torch.cat((pred, t.view(1, -1)))

            loss = criterion(pred, target)
            loss.backward()
            loss_record.append(loss.item())
            optimizer.step()

            pred = torch.softmax(pred, dim=-1).detach().cpu()
            target = target.cpu()
            pred = torch.argmax(pred, dim=-1)
            acc = (pred.numpy() == target.numpy()).mean()

            train_pbar.set_postfix({"loss": f"{loss:.3f}", "acc": f"{acc:.3f}"})
        
        train_loss = sum(loss_record) / len(loss_record)

        model.eval()

        loss_record = []
        acc = 0
        eval_pbar = tqdm(evaluating_loader, position=0, leave=True, desc=f"Evaluating: [{epoch+1}/{args.num_epoch}]")

        for batch in eval_pbar:
            with torch.no_grad():
                pred_ = model(batch)
                target = torch.LongTensor([label for _, label in batch.values()]).to(args.device)

                pred = torch.Tensor([]).to(args.device)
                for t in pred_.values():
                    pred = torch.cat((pred, t.view(1, -1)))

                loss = criterion(pred, target)
                loss_record.append(loss.item())

                pred = torch.softmax(pred, dim=-1).detach().cpu()
                target = target.cpu()
                pred = torch.argmax(pred, dim=-1)
                acc += (pred.numpy() == target.numpy()).mean()

        eval_loss = sum(loss_record) / len(loss_record)
        acc = acc / len(evaluating_loader)

        print(f'Epoch: [{epoch+1}/{args.num_epoch}] | Train loss: {train_loss:.4f}, Eval loss: {eval_loss:.4f}, Eval acc: {acc:.4f}')

        if eval_loss < best_loss:
            best_loss = eval_loss
            torch.save(model.state_dict(), args.ckpt_dir / "model.ckpt")
            print(f'Saving model with loss {best_loss:.3f} !!!')

    print("TRAINING COMPLETED !!!")


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--data_dir",
        type=Path,
        help="Directory to the dataset.",
        default="./data/intent/",
    )
    parser.add_argument(
        "--cache_dir",
        type=Path,
        help="Directory to the preprocessed caches.",
        default="./cache/intent/",
    )
    parser.add_argument(
        "--ckpt_dir",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpt/intent/",
    )

    # data
    parser.add_argument("--max_len", type=int, default=28)

    # model
    parser.add_argument("--hidden_size", type=int, default=512)
    parser.add_argument("--num_layers", type=int, default=3)
    parser.add_argument("--dropout", type=float, default=0.1)
    parser.add_argument("--bidirectional", type=bool, default=True)

    # optimizer
    parser.add_argument("--lr", type=float, default=1e-3)

    # data loader
    parser.add_argument("--batch_size", type=int, default=128)

    # training
    parser.add_argument(
        "--device", type=torch.device, help="cpu, cuda, cuda:0, cuda:1", default="cuda:0"
    )
    parser.add_argument("--num_epoch", type=int, default=100)

    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    args.ckpt_dir.mkdir(parents=True, exist_ok=True)
    main(args)
