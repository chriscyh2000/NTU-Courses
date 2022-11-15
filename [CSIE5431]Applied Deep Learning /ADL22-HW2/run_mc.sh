#!/bin/bash

python preprocess_mc.py \
--train_file ./data/train.json \
--valid_file ./data/valid.json \
--test_file ./data/test.json \
--context_file ./data/context.json \
--tokenizer_name_or_path hfl/chinese-roberta-wwm-ext \
--max_length 512 \
--processed_data_dir ./processed_data/mc/

python train_mc.py \
--processed_data_dir ./processed_data/mc/ \
--pretrained_model_name_or_path hfl/chinese-roberta-wwm-ext \
--model_save_dir ./ckpt/trained/mc/ \
--lr 5e-5 \
--per_device_train_batch_size 1 \
--per_device_val_batch_size 1 \
--accumulate_step 16 \
--num_epoch 1

python test_mc.py \
--processed_data_dir ./processed_data/mc/ \
--model_save_dir ./ckpt/trained/mc/ \
--per_device_test_batch_size 1 \
--pred_file ./pred/relevant_pred.json