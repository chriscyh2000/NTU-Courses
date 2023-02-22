import torch

from torch import nn

class FeatureExtractorv1(nn.Module):
    def __init__(self, hidden_size=256, dropout=0.1):
        super(FeatureExtractorv1, self).__init__()
        self.hidden_size = hidden_size
        self.user_embed = nn.Sequential(
            nn.Linear(512, hidden_size), 
            nn.ReLU(),
            nn.Dropout(dropout)
        )
        self.course_embed = nn.Sequential(
            nn.Linear(512, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout)
        )
        self.attnmodule = nn.TransformerDecoderLayer(d_model=hidden_size, nhead=1, batch_first=True)
        self.dropout = nn.Dropout(dropout)
        self.pred_head = nn.Linear(hidden_size, hidden_size)

    def forward(self, obs):
        user_embed = self.user_embed(torch.tensor(obs['user_embed']).cuda())
        course_embed = self.course_embed(torch.tensor(obs['course_embed']).cuda())
        # print(user_embed.shape, course_embed.shape)

        output = self.attnmodule(course_embed, user_embed)
        output = self.dropout(output)
        if len(output.shape) == 3:
            output = self.pred_head(output)[:, -1, :]
        else:
            output = self.pred_head(output)[-1, :]
        return output