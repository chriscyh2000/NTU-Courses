import torch
import torch.nn as nn
import torch.nn.functional as F

class MultiHeadAttention(nn.Module):
    '''
    input:
        query   - shape:[batch_size, query_dim]
        key     - shape:[num_courses, key_dim]
        value   - shape:[num_courses, key_dim]
    output:
        output  - shape:[batch_size, output_dim]
        scores  - shape:[num_heads, batch_size, num_courses]
    reference: 
        https://aitechtogether.com/article/21105.html
    '''
    
    def __init__(self, query_dim, key_dim, output_dim, num_heads=1):
        super().__init__()
        
        self.key_dim = key_dim
        self.output_dim = output_dim
        self.num_heads = num_heads
        
        self.Wq = nn.Linear(in_features=query_dim, out_features=output_dim, bias=False) # W_q
        self.Wk = nn.Linear(in_features=key_dim, out_features=output_dim, bias=False)   # W_k
        self.Wv = nn.Linear(in_features=key_dim, out_features=output_dim, bias=False)   # W_v
    
    def forward(self, query, key, value=None):
        value = (key if value == None else value)
        
        q = self.Wq(query)  # [batch_size, output_dim]
        k = self.Wk(key)    # [num_courses, output_dim]
        v = self.Wv(value)  # [num_courses, output_dim]

        assert (self.output_dim % self.num_heads == 0)
        split_size = self.output_dim // self.num_heads
        q = torch.stack(torch.split(q, split_size, dim=1), dim=0) # [num_heads, batch_size,  output_dim/num_heads]
        k = torch.stack(torch.split(k, split_size, dim=1), dim=0) # [num_heads, num_courses, output_dim/num_heads]
        v = torch.stack(torch.split(v, split_size, dim=1), dim=0) # [num_heads, num_courses, output_dim/num_heads]
        
        scores = torch.matmul(q, k.transpose(-2, -1)) / (self.key_dim ** 0.5)   # [num_heads, batch_size, num_courses]
        scores = F.softmax(scores, dim=2)                                       # score = softmax((QK)/sqrt(key_dim))

        output = torch.matmul(scores, v)                                        # [num_heads, batch_size, output_dim/num_heads]
        output = torch.cat(torch.split(output, 1, dim=0), dim=2).squeeze(0)     # [batch_size, output_dim]
        
        return output, scores