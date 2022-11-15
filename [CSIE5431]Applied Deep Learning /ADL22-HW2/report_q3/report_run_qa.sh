#!/bin/bash

python preprocess_qa.py 

python report_train_qa.py \
--train_file ./processed_data/train.json \
--validation_file ./processed_data/valid.json \
--max_seq_length 512 \
--model_name_or_path ./ckpt/pretrained/macbert \
--model_save_dir ./.cache \
--pad_to_max_length \
--learning_rate 3e-5 \
--per_device_train_batch_size 1 \
--per_device_eval_batch_size 1 \
--gradient_accumulation_steps 2 \
--num_train_epochs 1 

python test_qa.py \
--test_file ./processed_data/test.json \
--max_seq_length 512 \
--model_name_or_path ./ckpt/trained/qa \
--pad_to_max_length 
