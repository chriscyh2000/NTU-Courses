import argparse
from text2vec import SentenceModel
import datasets
import pickle
import numpy as np
from tqdm import tqdm
import os

def getParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--data_path', type=str, default='processed_datas/train_users.json')
    return parser


def main():
    parser = getParser()
    args = parser.parse_args()

    ## check if data_path already exists
    out_path = f"{args.data_path.split('.')[0]}_text2vec.pkl"
    if os.path.exists(out_path):
        print(f"{out_path} already exists, skipping")
        return
    embedder = SentenceModel('shibing624/text2vec-base-chinese')

    dataset = datasets.Dataset.from_json(args.data_path)
    embeddings = []
    
    print(dataset[0]['text'])
    for i in tqdm(range(len(dataset))):
        input_text = dataset[i]['text']
        embedding = embedder.encode(input_text)
        embeddings.append(embedding)
        
    embeddings = np.array(embeddings)
    with open(out_path, 'wb') as f:
        pickle.dump(embeddings, f)

if __name__ == '__main__':
    main()


