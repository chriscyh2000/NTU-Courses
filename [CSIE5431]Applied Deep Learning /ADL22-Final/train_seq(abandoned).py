from transformers import EncoderDecoderModel, TrainingArguments, AutoTokenizer, Seq2SeqTrainer, DataCollatorForSeq2Seq
import transformers
import datasets

from encoder import BaseEncoder
from decoder import BaseSeqDecoder

import pandas as pd
### mode = ["seq", "val"]
mode = "seq"
data_path = "dummy_train.json"
model_name = "google/mt5-small"

encoder = BaseEncoder(model_name=model_name)
decoder = BaseSeqDecoder(vocab_size=732)
#config = EncoderDecoderModel.from_encoder_decoder_pretrained(encoder, decoder)
#model = EncoderDecoderModel(config=config)
model = EncoderDecoderModel(encoder=encoder, decoder=decoder)
model.config.decoder_start_token_id = 0
model.config.pad_token_id = 3

tokenizer = AutoTokenizer.from_pretrained(model_name)
dataset = datasets.load_dataset("json", data_files="dummy_train.json")

model.train()
model.to("cuda")

# "text", "label"
def collate_fn(examples):
    tokens = tokenizer(examples["text"], truncation=True, max_length=512)
    examples["input_ids"] = tokens["input_ids"]
    examples["attention_mask"] = tokens["attention_mask"]
    return examples

data_collator = DataCollatorForSeq2Seq(tokenizer, model=model, padding="longest", max_length=512)

dataset = dataset.map(collate_fn, batched=True)
dataset.set_format(type="torch", columns=["input_ids", "attention_mask", "labels"])

training_args = TrainingArguments(
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

trainer = Seq2SeqTrainer(
    model=model,
    args=training_args,
    train_dataset=dataset["train"],
    tokenizer=tokenizer,
    data_collator=data_collator,
)

trainer.train()

