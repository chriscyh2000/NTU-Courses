import transformers
import datasets

import torch
from encoder import BaseEncoder
from decoder import LSTMSeqDecoder, BaseSeqDecoder

from transformers import PreTrainedModel, PretrainedConfig
from transformers import AutoModel, AutoConfig
from multihead_attention import MultiHeadAttention
import pickle
from utils import pad_to
import torch.nn as nn
import torch.nn.functional as F

from multihead_attention import MultiHeadAttention

from utils import mapk

class Seq2SeqConfig(PretrainedConfig):
    model_type = 'seq2seq'
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        


class Seq2SeqModel(PreTrainedModel):
    def __init__(self, encoder, decoder, config=Seq2SeqConfig()):
        super().__init__(config)
        self.config = Seq2SeqConfig()
        self.encoder = encoder
        self.decoder = decoder
        self.loss_fn = torch.nn.CrossEntropyLoss()

        # after encoder [batch, seq = 128, dim = 512]
        self.user_pre_atten = nn.Linear(in_features=512, out_features=128, bias=False)
        self.cour_pre_atten = nn.Linear(in_features=512, out_features=128, bias=False)

        self.mh_attention = MultiHeadAttention(128, 128, output_dim=128, num_heads=1)

        self.start_token=0
        self.end_token=1

        

    def add_start_token(self, input_ids):
        return torch.cat([torch.ones(input_ids.shape[0], 1).long().cuda() * self.start_token, input_ids], dim=1)

    def add_end_token(self, input_ids):
        input_ids = torch.cat([input_ids, torch.ones(input_ids.shape[0], 1).long().cuda() * -100], dim=1)
        add_pos = torch.argmin(input_ids, dim=1)
        input_ids[torch.arange(input_ids.shape[0]), add_pos] = self.end_token
        return input_ids

    def forward(self, **inputs):
        # courses/users embedding, labels(purchased course id)
        input_ids = inputs["input_ids"]
        attention_mask = inputs["attention_mask"]
        labels = inputs.get("labels", None)
        encoder_outputs = self.encoder(input_ids=input_ids, attention_mask=attention_mask)
        # print(encoder_outputs.last_hidden_state)
        hidden_user = self.user_pre_atten(encoder_outputs)
        hidden_cour = self.cour_pre_atten(encoder_outputs)


        logits = self.decoder(encoder_outputs.last_hidden_state, labels=self.add_start_token(labels))
        # print(logits)
        if labels is not None:
            loss = self.loss_fn(logits.view(-1, logits.shape[-1]), self.add_end_token(labels).view(-1))
            # print(loss)
            # print(logits.shape)
            # print(logits)
            # print(labels)
            # use MAP@k
            loss = mapk(logits,labels)

            return logits, loss
        return logits, None

    def decode(self, **inputs):
        input_ids = inputs["input_ids"].cuda()
        attention_mask = inputs["attention_mask"].cuda()
        encoder_outputs = self.encoder(input_ids=input_ids, attention_mask=attention_mask)
        results = self.decoder.greedy_decode(encoder_outputs.last_hidden_state)
        return results
    def map_k(predicted, actual, k=10): # calculate score from predicted result and groundtruth of a user.
        # both predicted, actual are a list of course id.
        if len(predicted)>k:
            predicted = predicted[:k]

        score = 0.0
        num_hits = 0.0

        if not actual:
            return 0.0
        for i,p in enumerate(predicted):
            if p in actual and p not in predicted[:i]:
                num_hits += 1.0
                score += num_hits / (i+1.0)


        return score / min(len(actual), k)


class Seq2SeqWithAttentionModel(Seq2SeqModel):
    def __init__(self, encoder, decoder, config=Seq2SeqConfig(), course_embed_path="processed_datas/train_courses_embeddings.pkl"):
        super().__init__(encoder, decoder, config)
        self.config = config
        self.encoder = encoder
        self.decoder = decoder
        self.attention = MultiHeadAttention(512, 512, 512, 4)

        self.course_embed = pad_to(pickle.load(open(course_embed_path, "rb")))
        ### pad 4 special tokens with random values
        self.course_embed = torch.cat([torch.randn(4, 512, 512), self.course_embed], dim=0)
        self.course_embed = nn.Parameter(self.course_embed, requires_grad=False)
        self.loss_fn = torch.nn.CrossEntropyLoss()

        self.start_token=0

    def forward(self, **inputs):
        input_ids = inputs["input_ids"]
        attention_mask = inputs["attention_mask"]
        labels = inputs.get("labels", None)
        encoder_outputs = self.encoder(input_ids=input_ids, attention_mask=attention_mask)
        attn_results, scores = self.attention(encoder_outputs.last_hidden_state, self.course_embed)
        attn_results = attn_results.unsqueeze(1)
        logits = self.decoder(attn_results, labels=self.add_start_token(labels))
        if labels is not None:
            loss = self.loss_fn(logits.view(-1, logits.shape[-1]), self.add_end_token(labels).view(-1))
            return logits, loss
        return logits, None

class AttnCourseEmbed(nn.Module):
    def __init__(self, course_embed_path="processed_datas/train_courses_embeddings.pkl", output_dim=512, nhead=8):
        super().__init__()
        self.course_embed = pad_to(pickle.load(open(course_embed_path, "rb")))
        ### pad 4 special tokens with random values
        self.course_embed = torch.cat([torch.randn(4, 512, 512), self.course_embed], dim=0)
        self.course_embed = nn.Parameter(self.course_embed, requires_grad=False)
        total_course_num = self.course_embed.shape[0]
        self.course_global_attn_token = nn.Parameter(torch.randn(total_course_num, 1, 512), requires_grad=True)
        self.attention = nn.MultiheadAttention(512, nhead, dropout=0.1, batch_first=True)
        self.out_proj = nn.Sequential(
            nn.Linear(512, output_dim),
            nn.ReLU(),
            nn.Dropout(0.1)
        )

    def forward(self):
        attn_results, scores = self.attention(self.course_global_attn_token, self.course_embed, self.course_embed)
        return self.out_proj(attn_results.squeeze(1))

class ConvCourseEmbed(nn.Module):
    def __init__(self, course_embed_path="processed_datas/train_courses_embeddings.pkl", out_dim=256, **kwargs):
        super().__init__()
        self.course_embed = pad_to(pickle.load(open(course_embed_path, "rb")))
        self.course_embed = torch.cat([torch.randn(4, 512, 512), self.course_embed], dim=0)
        self.course_embed = nn.Parameter(self.course_embed.permute(0, 2, 1), requires_grad=False)
        total_course_num = self.course_embed.shape[0]
        ### [total_course_num, C, L]
        self.conv = nn.Sequential(
            nn.Conv1d(512, 256, 9, padding=4, stride=4),
            nn.ReLU(),
            nn.Dropout(0.1),
            nn.Conv1d(256, 128, 9, padding=4, stride=4),
            nn.ReLU(),
            nn.Dropout(0.1),
            nn.Conv1d(128, 64, 9, padding=4, stride=4),
            nn.ReLU(),
            nn.Dropout(0.1),
        )

        self.out_proj = nn.Sequential(
            nn.Linear(64*8, out_dim),
            nn.ReLU(),
            nn.Dropout(0.1)
        )
    ### [total_course_num, 64, 8]
    def forward(self):
        x = self.conv(self.course_embed).flatten(1)
        return self.out_proj(x)

class CompressedCourseEmbed(nn.Module):
    def __init__(self, course_embed_path="processed_datas/compressed_courses_embeddings.pkl", out_dim=256, **kwargs):
        super().__init__()
        self.course_embed = torch.from_numpy(pickle.load(open(course_embed_path, "rb")))
        self.course_embed = torch.cat([torch.randn(4, 512), self.course_embed], dim=0)
        self.course_embed = nn.Parameter(self.course_embed, requires_grad=False)
        total_course_num = self.course_embed.shape[0]
        self.out_proj = nn.Sequential(
            nn.Linear(512, out_dim),
            nn.ReLU(),
            nn.Dropout(0.1)
        )
    def forward(self):
        return self.out_proj(self.course_embed)

class CourseEmbed1227(nn.Module):
    def __init__(self, course_embed_path="processed_datas/train_course_embeds.pkl", out_dim=256, **kwargs):
        super().__init__()
        self.course_embed = torch.from_numpy(pickle.load(open(course_embed_path, "rb")))
        self.course_embed = torch.cat([torch.randn(4, 768), self.course_embed], dim=0)
        self.course_embed = nn.Parameter(self.course_embed, requires_grad=False)
        total_course_num = self.course_embed.shape[0]
        self.out_proj = nn.Sequential(
            nn.Linear(768, out_dim),
            nn.ReLU(),
            nn.Dropout(0.1)
        )
    def forward(self):
        return self.out_proj(self.course_embed) 

class ValModelMLP(nn.Module):
    def __init__(self, hidden_size=256, dropout=0.1, in_size=512, nhead=4):
        super().__init__()
        self.course_embed = nn.Parameter(torch.randn(732, hidden_size), requires_grad=True)
            
        user_embed_path = "processed_datas/word_embeds.pkl"
        with open(user_embed_path, "rb") as f:
            user_embed = torch.from_numpy(pickle.load(f))
        self.user_embed = nn.Parameter(user_embed, requires_grad=False)

        self.user_project = nn.Linear(768, hidden_size)

        # self.user_project = nn.Sequential(
        #     nn.Linear(in_size, hidden_size),
        #     nn.ReLU(),
        #     nn.Dropout(dropout), 
        #     nn.Linear(hidden_size, hidden_size),
        #     nn.ReLU(),
        #     nn.Dropout(dropout)
        # )
        self.attention = nn.MultiheadAttention(hidden_size, nhead, dropout=dropout, batch_first=True)

        self.out_proj = nn.Sequential(
            nn.Linear(hidden_size, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout),
            nn.Linear(hidden_size, 1)
        )

    def forward(self, user_embed):
        user_embed = self.user_project(user_embed.permute(0, 2, 1) * self.user_embed)
        #.repeat(1, self.course_embed.shape[0], 1)
        course_embed = self.course_embed.repeat(user_embed.shape[0], 1, 1)
        output, _ = self.attention(course_embed, user_embed, user_embed)
        return self.out_proj(output).squeeze(-1)



class ValModelv0(nn.Module):
    def __init__(self, hidden_size=256, dropout=0.1, nhead=1, in_size=512):
        super().__init__()
        self.course_embed = nn.Parameter(torch.randn(732, hidden_size), requires_grad=True)

        self.course_project = nn.LayerNorm(hidden_size)

        self.user_project = nn.Sequential(
            nn.Linear(in_size, hidden_size),
        )

        self.attention = nn.MultiheadAttention(hidden_size, nhead, batch_first=True, dropout=dropout)

        self.out_proj = nn.Sequential(
            nn.Linear(hidden_size, 1),
        )
    
    def forward(self, x):
        ### x: user_embed
        batched_course_embed = torch.stack([self.course_embed] * x.shape[0], dim=0)
        x = self.user_project(x)
        attn_results, scores = self.attention(batched_course_embed, x, x)

        attn_results *= batched_course_embed
    
        return self.out_proj(attn_results).squeeze(-1)


class ValModelv1(nn.Module):
    def __init__(self, hidden_size=256, dropout=0.1, nhead=1, in_size=512):
        super().__init__()
        self.course_embed = CourseEmbed1227(output_dim=hidden_size)

        self.course_project = nn.LayerNorm(hidden_size)

        self.user_project = nn.Sequential(
            nn.Linear(in_size, hidden_size),
            # nn.ReLU(),
            # nn.LayerNorm(hidden_size),
            # nn.Dropout(dropout)
        )

        self.attention = nn.MultiheadAttention(hidden_size, nhead, dropout=dropout, batch_first=True)

        self.attention2 = nn.MultiheadAttention(hidden_size, nhead, dropout=dropout, batch_first=True)

        self.out_proj = nn.Sequential(
            nn.Linear(hidden_size, 1),
            # nn.ReLU(),
            # nn.Dropout(dropout),
            # nn.Linear(hidden_size, 1),
        )
    
    def forward(self, x):
        ### x: user_embed
        unbatched_course_embed = self.course_embed()
        
        batched_course_embed = self.course_project(torch.repeat_interleave(unbatched_course_embed.unsqueeze(0), x.shape[0], dim=0))
        x = self.user_project(x)
        attn_results, scores = self.attention(batched_course_embed, x, x)

        attn_results = self.attention2(attn_results, x, x)[0] + attn_results
        
        return self.out_proj(attn_results).squeeze(-1)

class ValModelv2(nn.Module):  
    def __init__(self, hidden_size=256, dropout=0.1, nhead=1):
        super().__init__()
        self.course_embed = nn.Parameter(torch.randn(732, hidden_size), requires_grad=True)

        self.user_project = nn.Sequential(
            nn.Linear(512, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout)
        )
        self.user_global_attn_token = nn.Parameter(torch.randn(1, 1, hidden_size), requires_grad=True)
        self.user_attention = nn.MultiheadAttention(hidden_size, 1, dropout=dropout, batch_first=True)

        self.out_proj = nn.Sequential(
            nn.Linear(hidden_size, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout),
            nn.Linear(hidden_size, 1),
        )  

    def forward(self, x):
        ### x: user_embed
        # batched_course_embed = torch.stack([self.course_embed] * x.shape[0], dim=0)
        x = self.user_project(x)
        batched_global_attn_token = torch.cat([self.user_global_attn_token] * x.shape[0], dim=0)
        user_attn_results, scores = self.user_attention(batched_global_attn_token, x, x)

        ### user_attn_results: [batch_size, hidden_size]
        ### course_embed: [batch_size, course_num, hidden_size]
        ### calculate cosine similarity
        # user_attn_results = user_attn_results.unsqueeze(1)
        # course_embed = batched_course_embed
        cosine_similarity = F.cosine_similarity(user_attn_results, self.course_embed, dim=-1)
        return cosine_similarity

class ValModelv3(nn.Module):  
    def __init__(self, hidden_size=256, dropout=0.1, nhead=1):
        super().__init__()
        self.course_embed = AttnCourseEmbed(output_dim=hidden_size)

        self.user_project = nn.Sequential(
            nn.Linear(512, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout)
        )
        self.user_global_attn_token = nn.Parameter(torch.randn(1, 1, hidden_size), requires_grad=True)
        self.user_attention = nn.MultiheadAttention(hidden_size, 1, dropout=dropout, batch_first=True)

        self.out_proj = nn.Sequential(
            nn.Linear(hidden_size, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout),
            nn.Linear(hidden_size, 1),
        )  

    def forward(self, x):
        ### x: user_embed
        # batched_course_embed = torch.stack([self.course_embed] * x.shape[0], dim=0)
        x = self.user_project(x)
        batched_global_attn_token = torch.cat([self.user_global_attn_token] * x.shape[0], dim=0)
        user_attn_results, scores = self.user_attention(batched_global_attn_token, x, x)

        ### user_attn_results: [batch_size, hidden_size]
        ### course_embed: [batch_size, course_num, hidden_size]
        ### calculate cosine similarity
        # user_attn_results = user_attn_results.unsqueeze(1)
        # course_embed = batched_course_embed
        cosine_similarity = F.cosine_similarity(user_attn_results, self.course_embed(), dim=-1)
        return cosine_similarity

class ValModelv4(nn.Module):
    def __init__(self, hidden_size=256, dropout=0.1, nhead=1):
        super().__init__()
        self.course_embed = nn.Parameter(torch.randn(732, hidden_size), requires_grad=True)

        self.user_project = nn.Sequential(
            nn.Linear(512, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout)
        )

        self.attention1 = nn.MultiheadAttention(hidden_size, nhead, dropout=dropout, batch_first=True)

        self.proj1 = nn.Sequential(
            nn.Linear(hidden_size, hidden_size*4),
            nn.ReLU(),
            nn.Dropout(dropout), 
            nn.Linear(hidden_size*4, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout),
        )

        self.attention2 = nn.MultiheadAttention(hidden_size, nhead, dropout=dropout, batch_first=True)

        self.out_proj = nn.Sequential(
            nn.Linear(hidden_size, hidden_size),
            nn.ReLU(),
            nn.Dropout(dropout),
            nn.Linear(hidden_size, 1),
        )
    
    def forward(self, x):
        ### x: user_embed
        batched_course_embed = torch.stack([self.course_embed] * x.shape[0], dim=0)
        x = self.user_project(x)
        attn_results, scores = self.attention1(batched_course_embed, x, x)
        attn_results = self.proj1(attn_results)
        attn_results, scores = self.attention2(batched_course_embed, attn_results, attn_results)
        
        return self.out_proj(attn_results).squeeze(-1)
    
    
class seq2seqModel_MeanPoolingAttn(Seq2SeqModel):
    def __init__(
        self, model_name, 
        config=Seq2SeqConfig(), 
        hidden_dim=512, 
        query_dim=128, 
        key_dim=128, 
        output_dim=512, 
        num_heads=4, 
        total_courses=732
    ):
        
        super().__init__(BaseEncoder(model_name=model_name), BaseSeqDecoder(vocab_size=total_courses), config)
        self.user_linear_layer = nn.Linear(hidden_dim, query_dim)
        self.course_linear_layer = nn.Linear(hidden_dim, key_dim)
        self.mh_attention = MultiHeadAttention(query_dim, key_dim, output_dim, num_heads)
        
        self.encoder.requires_grad_(False)
            
    def forward(self, **inputs):
        user_input_ids = inputs["user_input_ids"]
        course_input_ids = inputs["course_input_ids"]
        labels = inputs.get("labels", None)
        
        user_embeddings = self.user_linear_layer(user_input_ids)
        course_embeddings = self.course_linear_layer(course_input_ids)
        
        attention_output, _ = self.mh_attention(user_embeddings, course_embeddings, course_embeddings)
        attention_output = attention_output.unsqueeze(1)
        logits = self.decoder(attention_output, labels=self.add_start_token(labels))
        
        if labels is not None:
            loss = self.loss_fn(logits.view(-1, logits.shape[-1]), self.add_end_token(labels).view(-1))
            return logits, loss
        return logits, None

class Attn_valuebase(nn.Module):
    def __init__(
        self,
        hidden_dim=512, 
        query_dim=128, 
        key_dim=128, 
        output_dim=512, 
        num_heads=4, 
        total_courses=728
    ):
        
        super().__init__()
        self.loss_fn = nn.BCELoss()
        self.user_linear_layer = nn.Linear(hidden_dim, query_dim)
        self.course_linear_layer = nn.Linear(hidden_dim, key_dim)
        self.mh_attention = MultiHeadAttention(query_dim, key_dim, output_dim, num_heads)
        self.conv = nn.Conv1d(num_heads, 1, kernel_size=1, stride=1)
            
    def forward(self, output_result=False, users_seen=False, **inputs):
        user_input_ids = inputs["user_input_ids"]
        course_input_ids = inputs["course_input_ids"]
        labels = inputs.get("labels", None)
        unseen_labels = inputs.get("unseen_labels", None)
        
        user_embeddings = self.user_linear_layer(user_input_ids)
        course_embeddings = self.course_linear_layer(course_input_ids)
        
        _, score = self.mh_attention(user_embeddings, course_embeddings, course_embeddings)

        score = score.transpose(0, 1)
        score = self.conv(score).squeeze(1)
        score = torch.softmax(score, dim=-1)

        if users_seen == False:
            result = torch.topk(score, 50, dim=-1).indices.tolist()
            
            if labels is not None:
                ori_labels = []
                for label in labels:
                    ori_labels.append(label.nonzero().squeeze(-1).tolist())
                batch_score = mapk(ori_labels, result)
                
                # labels = labels / torch.sum(labels, dim=-1).unsqueeze(1)
                loss = self.loss_fn(score, labels)
                
                if output_result: return score, loss, result, batch_score
                else: return score, loss, batch_score
                
            if output_result: return score, None, result
            else: return score, None
            

        else:
            result = torch.topk(score, 100, dim=-1).indices.tolist()
            if unseen_labels is not None:
                seen_labels = []
                for label in labels:
                    seen_labels.append(label.nonzero().squeeze(-1).tolist())

                for i ,(pred_label, seen_label) in enumerate(zip(result, seen_labels)):
                    result[i] = [l for l in pred_label if l not in seen_label]
                batch_score = mapk(unseen_labels, result)
                
                # labels = labels / torch.sum(labels, dim=-1).unsqueeze(1)
                loss = self.loss_fn(score, labels)
                
                if output_result: return score, loss, result, batch_score
                else: return score, loss, batch_score
                
            if output_result: return score, None, result
            else: return score, None
