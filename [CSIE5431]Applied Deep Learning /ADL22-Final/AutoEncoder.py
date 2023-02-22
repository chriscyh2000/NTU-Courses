import torch
from torch import nn

### input shape: (732, 512, 512)
### target shape: (732, 1, 512)


class Encoder(nn.Module):
    def __init__(self, input_size=512, hidden_size=2048, latent_size=1):
        super(Encoder, self).__init__()
        
        self.encode_content = nn.Sequential(
            nn.Linear(512, 2048),
            nn.ReLU(),
            nn.Linear(2048, 32),
        )

        self.encode_time = nn.Sequential(
            nn.Linear(32*512, 2048),
            nn.ReLU(),
            nn.Linear(2048, 512),
        )


    def forward(self, x):
        ### in: bs, 512, 512
        x = self.encode_content(x) ## out: bs, 512, 32
        x = x.view(x.size(0), -1) ### out: bs, 32, 512
        x = self.encode_time(x) ### out: bs, 32, 16
        return x

class Decoder(nn.Module):
    def __init__(self, input_size=1, hidden_size=2048, output_size=512):
        super(Decoder, self).__init__()
        self.decode_time = nn.Sequential(
            nn.Linear(512, 2048),
            nn.ReLU(),
            nn.Linear(2048, 32*512),
        )

        self.decode_content = nn.Sequential(
            nn.Linear(32, 2048),
            nn.ReLU(),
            nn.Linear(2048, 512),
        )



    def forward(self, x):
        x = self.decode_time(x) ### out: bs, 32*512
        x = x.view(x.size(0), 512, 32) ### out: bs, 512, 32
        x = self.decode_content(x) ### out: bs, 512, 512
        return x


class AutoEncoder(nn.Module):
    def __init__(self):
        super(AutoEncoder, self).__init__()
        # self.input_size = input_size
        # self.hidden_size = hidden_size
        # self.latent_size = latent_size
        
        self.encoder = Encoder()
        self.decoder = Decoder()

    def forward(self, x):
        x = self.encoder(x)
        x = self.decoder(x)
        return x