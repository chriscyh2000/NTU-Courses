from encoder import BaseEncoder
import argparse
import torch.nn.functional as F
from transformers import AutoTokenizer, DataCollatorWithPadding
import datasets
import pickle
import torch
from tqdm import tqdm
import os

def getParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_name', type=str, default='google/mt5-small')
    parser.add_argument('--data_path', type=str, default='processed_datas/train_users.json')
    parser.add_argument('--seq_maxlen', type=int, default=512)

    return parser


def main():
    parser = getParser()
    args = parser.parse_args()
    ## check if data_path already exists
    out_path = f"processed_datas/{args.data_path.split('/')[-1].split('.')[0]}_embeddings.pkl"
    if os.path.exists(out_path):
        print(f"{out_path} already exists, skipping")
        return

    device = 'cuda:0' if torch.cuda.is_available() else 'cpu'
    
    encoder = BaseEncoder(model_name=args.model_name).to(device)
    encoder.requires_grad_(False)
    encoder.eval()
    tokenizer = AutoTokenizer.from_pretrained(args.model_name)

    dataset = datasets.Dataset.from_json(args.data_path)

    def collate_fn(examples):
        tokens = tokenizer(
            examples["text"], 
            padding='max_length', 
            truncation=True, 
            max_length=args.seq_maxlen
        )
        examples["input_ids"] = tokens["input_ids"]
        examples["attention_mask"] = tokens["attention_mask"]
        return examples

    dataset = dataset.map(collate_fn, batched=True, batch_size=4)
    embeddings = []
    for i in tqdm(range(len(dataset))):
        input_ids = dataset[i]['input_ids']
        attention_mask = dataset[i]['attention_mask']
        input_ids = torch.tensor(input_ids).unsqueeze(0).to(device)
        attention_mask = torch.tensor(attention_mask).unsqueeze(0).to(device)
        embedding = encoder(input_ids=input_ids, attention_mask=attention_mask).last_hidden_state
        # embedding = torch.transpose(embedding.squeeze(0), 0, 1)
        # embedding = F.avg_pool1d(embedding, kernel_size=embedding.shape[-1], stride=embedding.shape[-1])
        # embeddings.append(torch.transpose(embedding, 0, 1).squeeze(0).detach().cpu().numpy())
        embeddings.append(embedding.squeeze(0).detach().cpu().numpy())
    with open(out_path, 'wb') as f:
        pickle.dump(embeddings, f)

if __name__ == '__main__':
    main()


