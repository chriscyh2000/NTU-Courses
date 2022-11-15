import json
from datasets import DatasetDict, arrow_dataset
from pathlib import Path
import torch

# multiple-choice dataset
def MC_Dataset(dataset_file: Path) -> arrow_dataset.Dataset:
    dataset_dic = DatasetDict(json.loads(dataset_file.read_text()))
    data_len = len(list(dataset_dic.values())[-1])
    data_arr = [
        {key: torch.tensor(dataset_dic[key][i]) for key in dataset_dic.keys()} 
        for i in range(data_len)
    ]
    return data_arr