#!/bin/bash
python preprocess_qa.py \
--train_file ./data/train.json \
--valid_file ./data/valid.json \
--test_file ./data/test.json \
--context_file ./data/context.json \
--processed_data_dir ./processed_data/qa/ \
--relevant_pred_file ./pred/relevant_pred.json 

python train_qa.py \
--train_file ./processed_data/qa/train.json \
--validation_file ./processed_data/qa/valid.json \
--max_seq_length 512 \
--pretrained_model_name_or_path hfl/chinese-macbert-large \
--model_save_dir ./ckpt/trained/qa \
--pad_to_max_length \
--learning_rate 3e-5 \
--per_device_train_batch_size 1 \
--per_device_eval_batch_size 1 \
--gradient_accumulation_steps 2 \
--num_train_epochs 1 

python test_qa.py \
--tokenizer_name_or_path hfl/chinese-macbert-large \
--model_save_dir ./ckpt/trained/qa \
--test_file ./processed_data/qa/test.json \
--max_seq_length 512 \
--pad_to_max_length 
