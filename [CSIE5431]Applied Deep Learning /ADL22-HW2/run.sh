#!/bin/bash
# reproduce the predicting result

# "${1}": path to the context file.
# "${2}": path to the testing file.
# "${3}": path to the output predictions.

python preprocess_mc.py \
--context_file $1 \
--test_file $2 \
--tokenizer_name_or_path hfl/chinese-roberta-wwm-ext \
--max_length 512 \
--processed_data_dir ./processed_data/mc/

python test_mc.py \
--processed_data_dir ./processed_data/mc/ \
--model_save_dir ./ckpt/trained/mc/ \
--per_device_test_batch_size 1 \
--pred_file ./pred/relevant_pred.json

python preprocess_qa.py \
--context_file $1 \
--test_file $2 \
--processed_data_dir ./processed_data/qa/ \
--relevant_pred_file ./pred/relevant_pred.json

python test_qa.py \
--tokenizer_name_or_path hfl/chinese-macbert-large \
--model_save_dir ./ckpt/trained/qa \
--test_file ./processed_data/qa/test.json \
--max_seq_length 512 \
--pad_to_max_length \
--pred_file $3