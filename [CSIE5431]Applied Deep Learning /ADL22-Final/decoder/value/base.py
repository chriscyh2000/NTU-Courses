import torch

class BaseValDecoder(torch.nn.Module):
    def __init__(self, embed_dim=512, n_classes=764):
        super(BaseValDecoder, self).__init__()
        self.pool = torch.nn.AdaptiveAvgPool1d(1)
        self.projection = torch.nn.Linear(embed_dim, n_classes)
        self.sigmoid = torch.nn.Sigmoid()

    def forward(self, x):
        x = self.pool(x.permute(0, 2, 1)).squeeze(-1)
        x = self.projection(x)
        x = self.sigmoid(x)
        return x
        