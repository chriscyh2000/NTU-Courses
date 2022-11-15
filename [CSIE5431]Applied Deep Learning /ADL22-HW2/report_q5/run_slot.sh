CUDA_VISIBLE_DEVICES=1 python3 run_slot.py \
  --model_name_or_path bert-base-uncased \
  --train_file ./processed_data/slot/train.json \
  --validation_file ./processed_data/slot/eval.json \
  --output_dir /tmp/test-ner \
  --do_train \
  --do_eval