import torch
import torch.nn as nn
from torch.utils.data import DataLoader

import datasets, transformers
from transformers import AutoTokenizer, Trainer

from encoder import BaseEncoder
from decoder import BaseValDecoder

total_courses = 732
mode = "val"
data_path = "dummy_train.json"
model_name = "google/mt5-small"

encoder = BaseEncoder(model_name=model_name)
decoder = BaseValDecoder(n_classes=total_courses)

class RegressionModel(nn.Module):
    def __init__(self, encoder, decoder):
        super().__init__()
        self.encoder = encoder
        self.decoder = decoder
        self.loss_fn = nn.MSELoss()

    def forward(self, input_ids, attention_mask, labels=None):
        encoder_outputs = self.encoder(input_ids=input_ids, attention_mask=attention_mask)
        logits = self.decoder(encoder_outputs.last_hidden_state)
        if labels is not None:
            loss = self.loss_fn(logits, labels.float())
            return logits, loss
        return logits, None

model = RegressionModel(encoder, decoder)

tokenizer = AutoTokenizer.from_pretrained(model_name)
dataset = datasets.load_dataset("json", data_files="dummy_train.json")

def calc_label_values(labels, total_labels=total_courses):
    output = [0] * total_labels
    for i, label in enumerate(labels):
        output[label] = (len(labels)-i)/len(labels)
    return output

def collate_fn(examples):
    tokens = tokenizer(examples["text"], truncation=True, max_length=512)
    examples["input_ids"] = tokens["input_ids"]
    examples["attention_mask"] = tokens["attention_mask"]
    examples["labels"] = [calc_label_values(label) for label in examples["labels"]]
    return examples

data_collator = transformers.DataCollatorForSeq2Seq(tokenizer=tokenizer, model=model, padding="longest", max_length=512)

dataset = dataset.map(collate_fn, batched=True)
dataset.set_format(type="torch", columns=["input_ids", "attention_mask", "labels"])

class RegressionTrainer(Trainer):
    def compute_loss(self, model, inputs, return_outputs=False):
        labels = inputs.get("labels")
        logits, loss = model(**inputs)
        if return_outputs:
            return loss, logits
        return loss
        

training_args = transformers.TrainingArguments(
    output_dir="./results",
    num_train_epochs=1,
    per_device_train_batch_size=1,
    per_device_eval_batch_size=1,
    warmup_steps=500,
    weight_decay=0.01,
    logging_dir="./logs",
    logging_steps=10,
    save_steps=1000,
    save_total_limit=2,
    learning_rate=1e-4,
    fp16=True,
)

trainer = RegressionTrainer(
    model=model,
    args=training_args,
    train_dataset=dataset["train"],
    tokenizer=tokenizer,
    data_collator=data_collator,
)


trainer.train()


