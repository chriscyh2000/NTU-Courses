from transformers import AutoModel, AutoConfig
from torch import nn

class BaseEncoder(nn.Module):

    def __init__(self, model_name="google/mt5-small", max_length=256, freeze=True):
        super(BaseEncoder, self).__init__()
        self.encoder = AutoModel.from_pretrained(model_name).get_encoder()
        self.max_length = max_length
        if freeze:
            for param in self.encoder.parameters():
                param.requires_grad = False
        self.config = AutoConfig.from_pretrained(model_name)

    def forward(self, *args, **kwargs):
        return self.encoder(*args, **kwargs)

    def get_output_embeddings(self):
        return None

    