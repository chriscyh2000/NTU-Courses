from transformers import AutoTokenizer

def preprocess(
    tokenizer: AutoTokenizer,
    examples, 
    split: str = "train",
    max_input_length: int = 256,
    max_target_length: int = 64,
):
    inputs = [inp for inp in examples["maintext"]]
    model_inputs = tokenizer(
        inputs,
        max_length=max_input_length,
        padding="max_length",
        truncation=True,
    )
    
    if split == "train":
        labels = tokenizer(
            examples["title"],
            max_length=max_target_length,
            padding="max_length",
            truncation=True,
        )

        labels["input_ids"] = [
            [(l if l != tokenizer.pad_token_id else -100) for l in label]
            for label in labels["input_ids"]
        ]
        model_inputs["labels"] = labels["input_ids"]

    return model_inputs
