import torch
from transformers import MT5ForConditionalGeneration, MT5Tokenizer, MT5Config

class BaseSeqDecoder(torch.nn.Module):
    
    def __init__(self, vocab_size):
        super(BaseSeqDecoder, self).__init__()
        config = MT5Config()
        config.vocab_size = vocab_size
        config.num_decoder_layers = 2
        model = MT5ForConditionalGeneration(config=config)
        self.decoder = model.get_decoder()
        self.lm_head = model.lm_head
        self.config = config

    def forward(self, embeddings, labels):
        ### replace -100 with 3(pad)
        labels[labels == -100] = 3
        decoder_outputs = self.decoder(labels, encoder_hidden_states=embeddings)
        return self.lm_head(decoder_outputs[0])

    def greedy_decode(self, embeddings, max_len=50):
        # greedy decoding
        # x: [batch, seq_len, embed_dim]
        # return: [batch, seq_len]
        batch_size = embeddings.size(0)
        output = torch.zeros(batch_size, max_len).long().to(embeddings.device)
        decoded_token = torch.zeros(batch_size, 1, self.config.vocab_size).bool().to(embeddings.device)
        output[:, 0] = 0
        for i in range(1, max_len):
            decoder_outputs = self.decoder(output[:, :i], encoder_hidden_states=embeddings)
            output_ = self.lm_head(decoder_outputs[0][:, -1:, :])
            output_[decoded_token] = -1e9
            output_ = output_.argmax(-1)
            output[:, i] = output_.squeeze(-1)
            decoded_token[torch.arange(batch_size), 0, output_] = True
        return output



        
    

    

        

        