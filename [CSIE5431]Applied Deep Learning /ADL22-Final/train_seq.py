import transformers
import datasets

import torch
from encoder import BaseEncoder
from decoder import LSTMSeqDecoder, BaseSeqDecoder

total_courses = 732
data_path = "dummy_train.json"
model_name = "google/mt5-small"

encoder = BaseEncoder(model_name="google/mt5-small")

# decoder = LSTMSeqDecoder(vocab_size=total_courses)
decoder = BaseSeqDecoder(vocab_size=total_courses)

start_token=0
end_token=1


class Seq2SeqModel(torch.nn.Module):
    def __init__(self, encoder, decoder):
        super().__init__()
        self.encoder = encoder
        self.decoder = decoder
        self.loss_fn = torch.nn.CrossEntropyLoss()

    def add_start_token(self, input_ids):
        return torch.cat([torch.ones(input_ids.shape[0], 1).long().cuda() * start_token, input_ids], dim=1)

    def add_end_token(self, input_ids):
        input_ids = torch.cat([input_ids, torch.ones(input_ids.shape[0], 1).long().cuda() * -100], dim=1)
        add_pos = torch.argmin(input_ids, dim=1)
        input_ids[torch.arange(input_ids.shape[0]), add_pos] = end_token
        return input_ids

    def forward(self, input_ids, attention_mask, labels=None):
        encoder_outputs = self.encoder(input_ids=input_ids, attention_mask=attention_mask)
        logits = self.decoder(encoder_outputs.last_hidden_state, labels=self.add_start_token(labels))
        print(logits)
        if labels is not None:
            loss = self.loss_fn(logits.view(-1, logits.shape[-1]), self.add_end_token(labels).view(-1))
            return logits, loss
        return logits, None

model = Seq2SeqModel(encoder, decoder)

tokenizer = transformers.AutoTokenizer.from_pretrained(model_name)
dataset = datasets.load_dataset("json", data_files="dummy_train.json")

def collate_fn(examples):
    tokens = tokenizer(examples["text"], truncation=True, max_length=512)
    examples["input_ids"] = tokens["input_ids"]
    examples["attention_mask"] = tokens["attention_mask"]
    return examples

data_collator = transformers.DataCollatorForSeq2Seq(tokenizer=tokenizer, model=model, padding="longest", max_length=512)

dataset = dataset.map(collate_fn, batched=True)
dataset.set_format(type="torch", columns=["input_ids", "attention_mask", "labels"])

class MySeq2SeqTrainer(transformers.Seq2SeqTrainer):
    def compute_loss(self, model, inputs, return_outputs=False):
        labels = inputs.get("labels")
        logits, loss = model(**inputs)
        if return_outputs:
            return loss, logits
        return loss

training_args = transformers.TrainingArguments(
    output_dir="./results",
    num_train_epochs=1,
    per_device_train_batch_size=2,
    per_device_eval_batch_size=2,
    warmup_steps=500,
    weight_decay=0.01,
    logging_dir="./logs",
    logging_steps=10,
    save_steps=1000,
    save_total_limit=2,
    learning_rate=1e-4,
    fp16=True,
)

trainer = MySeq2SeqTrainer(
    model=model,
    args=training_args,
    train_dataset=dataset["train"],
    tokenizer=tokenizer,
    data_collator=data_collator,
)

trainer.train()