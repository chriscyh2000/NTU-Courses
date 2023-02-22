import torch
import numpy as np
from torch import nn
from torch.utils.data import DataLoader, Dataset, Subset
import transformers
import json, pickle
from models import *
from tqdm import tqdm
from utils import mapk
import os

class UserCourseDataset(Dataset):
    def __init__(self, users, user_embeds, total_courses=92):
        self.users = users
        self.user_embeds = [torch.from_numpy(user_embed) for user_embed in user_embeds]
        self.total_courses = total_courses

    def __len__(self):
        return len(self.users)

    def __getitem__(self, idx):
        labels = torch.zeros(self.total_courses)
        labels = labels.scatter_(0, torch.tensor(self.users[idx]["subgroups"]), 1)
        user_embed = self.user_embeds[idx]
        return user_embed, labels

def collate_fn(batch):
    ### pad the user_embeds to the same length
    # print(batch[0][0].shape)
    max_len = max([item[0].shape[0] for item in batch])
    embed_dim = batch[0][0].shape[1]
    user_embeds = torch.stack([torch.cat([item[0], torch.zeros(max_len - item[0].shape[0], embed_dim)], dim=0) for item in batch])
    labels = torch.stack([item[1] for item in batch])
    return user_embeds, labels

for prefix in ["test_unseen","test_seen"]:
	user_embed_path = f"reproduce/processed_datas/{prefix}_user_embeddings.pkl"
	user_path = f"reproduce/processed_datas/{prefix}_user.json"
	course_embed_path = "reproduce/processed_datas/train_courses_embeddings.pkl"

	with open(user_path, "r") as f:
	    users = json.load(f)

	with open(user_embed_path, "rb") as f:
	    user_embeds = pickle.load(f)

	dataset = UserCourseDataset(users, user_embeds)

	dataloader = DataLoader(dataset, batch_size=32, shuffle=False, collate_fn=collate_fn)
	model = ValModelv0(nhead=8, num_tokens=92, in_size=512).cuda()

	model.eval()
	total_samples = len(dataset)
	preds = np.zeros((total_samples, 92))

    model.load_state_dict(torch.load(f"ckpts/val_modelv0_2.pth"))
    with torch.no_grad():
        labels = []
        for i, (user_embed, label) in enumerate(tqdm(dataloader)):
            user_embed = user_embed.cuda()
            label = label.cuda()
            pred = model(user_embed)
            preds[i*32:(i+1)*32] = pred.cpu().numpy()
            
            ### transform the label to discrete numbers
            for l in label:
                labels.append(list(np.where(l.cpu()==1)[0]))
	        
	preds = torch.sigmoid(torch.tensor(preds))
	preds = preds.cpu().numpy()

	pred_labels = np.argsort(-preds, axis=1)[:, :50]
	print(mapk(labels, pred_labels, 50))
	with open(f"reproduce/results/{prefix.split("_")[-1]}_group_results.csv", "w") as f:
	    f.write(f"user_id,subgroup\n")
	    for i, pred in enumerate(pred_labels):
	        f.write(f"{users[i]['id']},")
	        for p in pred:
	            f.write(f"{p} ")
	        f.write("\n")