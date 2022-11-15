import json
import pickle
import math
from argparse import ArgumentParser, Namespace
from pathlib import Path
from typing import Dict

import torch
from torch.utils.data import DataLoader
from tqdm import tqdm

from dataset import SeqTaggingClsDataset
from model import SeqTagger
from utils import Vocab

# for report
from seqeval.scheme import IOB2
from seqeval.metrics import classification_report

TRAIN = "train"
DEV = "eval"
SPLITS = [TRAIN, DEV]


def main(args):
    # TODO: implement main function
    with open(args.cache_dir / "vocab.pkl", "rb") as f:
        vocab: Vocab = pickle.load(f)

    tag_idx_path = args.cache_dir / "tag2idx.json"
    tag2idx: Dict[str, int] = json.loads(tag_idx_path.read_text())

    data_paths = {split: args.data_dir / f"{split}.json" for split in SPLITS}
    data = {split: json.loads(path.read_text()) for split, path in data_paths.items()}
    datasets: Dict[str, SeqTaggingClsDataset] = {
        split: SeqTaggingClsDataset(split_data, vocab, tag2idx, args.max_len)
        for split, split_data in data.items()
    }

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
    model = SeqTagger(
        embeddings=embeddings,
        num_layers=args.num_layers,
        hidden_size=args.hidden_size,
        bidirectional=args.bidirectional,
        num_class=len(list(tag2idx.keys())),
        dropout=args.dropout,
        device=args.device
    ).to(args.device)

    optimizer = torch.optim.Adam(model.parameters(), lr=args.lr, weight_decay=2e-5)
    criterion = torch.nn.CrossEntropyLoss()

    best_loss = math.inf

    for epoch in range(args.num_epoch):
        model.train()
        loss_record = []

        train_pbar = tqdm(training_loader, position=0, leave=True, desc=f"Epoch: [{epoch+1}/{args.num_epoch}]")
        for batch in train_pbar:
            optimizer.zero_grad()

            pred_ = model(batch)
            pred = [probs for probs in pred_.values()]

            loss = None
            acc = 0
            batch_len = len(list(batch.values()))
            for i, (_, tags, _) in enumerate(batch.values()):
                target = torch.LongTensor(tags).to(args.device)
                if loss == None: loss = criterion(pred[i], target)
                else: loss += criterion(pred[i], target)

                pred_i = torch.softmax(pred[i].detach().cpu(), dim=-1)
                pred_i = torch.argmax(pred_i, dim=-1)
                target = target.cpu()
                acc += ((pred_i.numpy() == target.numpy()).mean() == 1)

            loss = loss / batch_len
            loss.backward()
            loss_record.append(loss.item())
            optimizer.step()

            acc = acc / batch_len
            train_pbar.set_postfix({"loss": f"{loss:.3f}", "acc": f"{acc:.3f}"})
        
        train_loss = sum(loss_record) / len(loss_record)

        loss_record = []
        eval_acc = 0
        eval_pbar = tqdm(evaluating_loader, position=0, leave=True, desc=f"Evaluating: [{epoch+1}/{args.num_epoch}]")

        y_true = []
        y_pred = []
        for batch in eval_pbar:
            with torch.no_grad():
                pred_ = model(batch)
                pred = [probs for probs in pred_.values()]

                loss = None
                acc = 0
                batch_len = len(list(batch.values()))
                for i, (_, tags, _) in enumerate(batch.values()):
                    target = torch.LongTensor(tags).to(args.device)
                    if loss == None: loss = criterion(pred[i], target)
                    else: loss += criterion(pred[i], target)

                    pred_i = torch.softmax(pred[i].detach().cpu(), dim=-1)
                    pred_i = torch.argmax(pred_i, dim=-1)
                    target = target.cpu()
                    acc += ((pred_i.numpy() == target.numpy()).mean() == 1)

                    # for report
                    y_true.append([datasets[DEV].idx2tag(t.item()) for t in target])
                    y_pred.append([datasets[DEV].idx2tag(p.item()) for p in pred_i])
                
                loss_record.append(loss.item() / batch_len)
                eval_acc += acc / batch_len

        eval_loss = sum(loss_record) / len(loss_record)
        eval_acc = eval_acc / len(evaluating_loader)
        print(f'Epoch: [{epoch+1}/{args.num_epoch}] | Train loss: {train_loss:.4f}, Eval loss: {eval_loss:.4f}, Eval acc: {eval_acc:.4f}')
        
        # for report
        print(classification_report(y_true, y_pred, mode='strict', scheme=IOB2))

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
        default="./data/slot/",
    )
    parser.add_argument(
        "--cache_dir",
        type=Path,
        help="Directory to the preprocessed caches.",
        default="./cache/slot/",
    )
    parser.add_argument(
        "--ckpt_dir",
        type=Path,
        help="Directory to save the model file.",
        default="./ckpt/slot/",
    )

    # data
    parser.add_argument("--max_len", type=int, default=35)

    # model
    parser.add_argument("--hidden_size", type=int, default=128)
    parser.add_argument("--num_layers", type=int, default=2)
    parser.add_argument("--dropout", type=float, default=0.1)
    parser.add_argument("--bidirectional", type=bool, default=True)

    # optimizer
    parser.add_argument("--lr", type=float, default=1e-3)

    # data loader
    parser.add_argument("--batch_size", type=int, default=128)

    # training
    parser.add_argument(
        "--device", type=torch.device, help="cpu, cuda, cuda:0, cuda:1", default="cpu"
    )
    parser.add_argument("--num_epoch", type=int, default=100)

    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    args.ckpt_dir.mkdir(parents=True, exist_ok=True)
    main(args)