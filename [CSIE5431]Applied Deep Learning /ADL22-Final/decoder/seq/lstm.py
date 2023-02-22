import torch.nn as nn
import torch


class LSTMSeqDecoder(nn.Module):
    def __init__(self, vocab_size, embed_dim=512, hidden_dim=512, n_layers=3, dropout=0.1):
        super(LSTMSeqDecoder, self).__init__()
        self.vocab_size = vocab_size
        self.hidden_dim = hidden_dim
        self.n_layers = n_layers
        self.embedding = nn.Embedding(vocab_size, embed_dim)
        self.lstm = nn.LSTM(input_size=embed_dim, hidden_size=hidden_dim, num_layers=n_layers, dropout=dropout, batch_first=True)
        self.fc = nn.Linear(hidden_dim, vocab_size)
        self.dropout = nn.Dropout(dropout)

        self.embed_pool = nn.AdaptiveAvgPool1d(1)
        self.embed_project = nn.Linear(embed_dim, hidden_dim)

    def encoder_projection(self, x):
        x = self.embed_pool(x.permute(0, 2, 1)).squeeze(-1)
        x = self.embed_project(x).unsqueeze(0)
        return x.repeat(self.n_layers, 1, 1)

    def forward(self, x, labels):
        ### replace -100 with 3(pad)
        labels[labels == -100] = 3
        embed = self.embedding(labels)
        embed = self.dropout(embed)

        h0 = self.encoder_projection(x)
        c0 = torch.zeros_like(h0)
        output, (hidden, cell) = self.lstm(embed, (h0, c0))

        output = self.fc(output)
        # "'last_hidden_state', 'past_key_values', 'logits'"
        return output

    def greedy_decode(self, x, max_len=50, start_token=0, end_token=1):
        # greedy decoding
        # x: [batch, seq_len, embed_dim]
        # return: [batch, seq_len]
        batch_size = x.size(0)
        hidden = self.encoder_projection(x)
        cell = torch.zeros_like(hidden)
        output = torch.zeros(batch_size, max_len).long().to(x.device)
        decoded_token = torch.zeros(batch_size, 1, self.vocab_size).bool().to(x.device)
        output[:, 0] = start_token
        ### not decoding repetitive token
        for i in range(1, max_len):
            embed = self.embedding(output[:, i-1])
            embed = self.dropout(embed)
            output_, (hidden, cell) = self.lstm(embed.unsqueeze(1), (hidden, cell))
            output_ = self.fc(output_)
            # mask previous decoded token
            output_[decoded_token] = -1e9
            output_ = output_.argmax(-1)
            output[:, i] = output_.squeeze(-1)
            decoded_token[torch.arange(batch_size), 0, output_] = True
        return output
