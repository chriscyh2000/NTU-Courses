from typing import Dict

import torch
import torch.nn as nn
from torch.nn import Embedding


class SeqClassifier(torch.nn.Module):
    def __init__(
        self,
        embeddings: torch.tensor,
        hidden_size: int = 256,
        num_layers: int = 1,
        dropout: float = 0.2,
        bidirectional: bool = True,
        num_class: int = 150,
        device: str = 'cuda:0'
    ) -> None:
        super(SeqClassifier, self).__init__()
        self.embed = Embedding.from_pretrained(embeddings, freeze=True) # freeze embedding
        self.device = device

        # TODO: model architecture
        self.lstm = nn.LSTM(
            input_size=self.embed(torch.LongTensor([0])).shape[1],
            hidden_size=hidden_size,
            num_layers=num_layers,
            dropout=dropout,
            bidirectional=bidirectional,
            batch_first=True,
        )
        lstm_output_size = hidden_size << (1 if bidirectional else 0)
        self.dnn = nn.Sequential(
            nn.Linear(lstm_output_size, lstm_output_size >> 1),
            nn.BatchNorm1d(lstm_output_size >> 1),
            nn.ReLU(),
            nn.Dropout(p=dropout),
            nn.Linear(lstm_output_size >> 1, lstm_output_size >> 2),
            nn.BatchNorm1d(lstm_output_size >> 2),
            nn.ReLU(),
            nn.Dropout(p=dropout),
            nn.Linear(lstm_output_size >> 2, num_class)
        )

    def forward(self, batch) -> Dict[str, torch.Tensor]:
        # TODO: implement model forward
        # batch: Dict(str, Tuple(List[int], str)) -> {instance_id: ([tokem_id...], intent_id)}
        input_tensor = torch.Tensor([]).to(self.device)
        for input, _ in batch.values():
            input_tensor = torch.cat((
                input_tensor, 
                torch.unsqueeze(self.embed(torch.LongTensor(input).to(self.device)), 0)
            ))

        output, _ = self.lstm(input_tensor)
        output = output[:, 0, :]
        output = self.dnn(output)

        output_dict = dict()
        for id, output_tensor in zip(batch.keys(), output):
            output_dict.update({id: output_tensor})
        
        return output_dict
        

class SeqTagger(SeqClassifier):
    def forward(self, batch: Dict) -> Dict[str, torch.Tensor]:
        # TODO: implement model forward
        input_tensor = torch.Tensor([]).to(self.device)
        for input, _, _ in batch.values():
            input_tensor = torch.cat((
                input_tensor, 
                torch.unsqueeze(self.embed(torch.LongTensor(input).to(self.device)), 0)
            ))

        output, _ = self.lstm(input_tensor)
        prev_shape = output.shape
        output = output.reshape(-1, prev_shape[-1])
        output = self.dnn(output)
        output = output.reshape(prev_shape[0], prev_shape[1], -1)

        output_dict = dict()
        for i, (id, (_, _, length)) in enumerate(batch.items()):
            output_dict.update({id: output[i, :length, :]})

        return output_dict
