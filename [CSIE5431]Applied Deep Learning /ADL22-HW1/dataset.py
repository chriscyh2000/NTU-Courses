from typing import List, Dict

from torch.utils.data import Dataset

from utils import Vocab


class SeqClsDataset(Dataset):
    def __init__(
        self,
        data: List[Dict],
        vocab: Vocab,
        label_mapping: Dict[str, int],
        max_len: int,
    ):
        self.data = data
        self.vocab = vocab
        self.label_mapping = label_mapping
        self._idx2label = {idx: intent for intent, idx in self.label_mapping.items()}
        self.max_len = max_len

    def __len__(self) -> int:
        return len(self.data)

    def __getitem__(self, index) -> Dict:
        instance = self.data[index]
        return instance

    @property
    def num_classes(self) -> int:
        return len(self.label_mapping)

    def collate_fn(self, samples: List[Dict]) -> Dict:
        # TODO: implement collate_fn
        batch_padded_ids = self.vocab.encode_batch(
            [sample["text"].split(" ") for sample in samples], 
            self.max_len
        )

        batch = dict()
        for sample, padded_ids in zip(samples, batch_padded_ids):
            batch.update({
                sample["id"]: (
                    padded_ids,                          # padded token_ids of input_data
                    self.label2idx(sample["intent"])     # output data
                    if "intent" in sample else None   
                )         
            })
        return batch

    def label2idx(self, label: str):
        return self.label_mapping[label]

    def idx2label(self, idx: int):
        return self._idx2label[idx]


class SeqTaggingClsDataset(SeqClsDataset):
    def __init__(
        self,
        data: List[Dict],
        vocab: Vocab,
        tag_mapping: Dict[str, int],
        max_len: int,
    ):
        self.data = data
        self.vocab = vocab
        self.tag_mapping = tag_mapping
        self._idx2tag = {id: tag for tag, id in tag_mapping.items()}
        self.max_len = max_len

    ignore_idx = -100

    def __len__(self) -> int:
        return len(self.data)

    def __getitem__(self, index) -> Dict:
        instance = self.data[index]
        return instance
    
    def collate_fn(self, samples):
        # TODO: implement collate_fn
        batch = dict()
        batch_padded_ids = self.vocab.encode_batch(
            [sample["tokens"] for sample in samples],
            self.max_len
        )
        for sample, padded_ids in zip(samples, batch_padded_ids):
            batch.update({
                sample["id"]: (
                    padded_ids,                                     # padded token_ids of input_data
                    [self.tag2idx(tag) for tag in sample["tags"]]   # output data
                    if "tags" in sample else None,
                    len(sample["tokens"])
                )
            })
        return batch

    def tag2idx(self, label: str):
        return self.tag_mapping[label]
    
    def idx2tag(self, idx: int):
        return self._idx2tag[idx]
