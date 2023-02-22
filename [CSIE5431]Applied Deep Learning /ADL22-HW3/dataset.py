from pathlib import Path
from datasets import Dataset, DatasetDict
import jsonlines as jl

def loadDatasets(
    path: Path, 
    test_size: float = 0.2, 
    split: str = "train",
    other_split: str = "val"
):

    with jl.open(path) as reader:
        data_array = [obj for obj in reader]
    
    keys = ["title", "maintext", "id"] if split == "train" else ["maintext", "id"]
    data_dict = {
        key: [obj[key] for obj in data_array] 
        for key in keys
    } 
    
    dataset = Dataset.from_dict(data_dict)

    if test_size > 0.0:
        dataset_dict = dataset.train_test_split(test_size)
        return DatasetDict(
            {split: dataset_dict["train"], other_split: dataset_dict["test"]}
        )

    dataset_dict =  DatasetDict({split: dataset})
    return dataset_dict
