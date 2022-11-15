CUDA_VISIBLE_DEVICES=1 python3 run_intent.py \
  --model_name_or_path bert-base-uncased \
  --do_train \
  --do_eval \
  --num_train_epochs 2 \
  --train_file ./processed_data/intent/train.json \
  --validation_file ./processed_data/intent/eval.json \
  --ignore_mismatched_sizes \
  --output_dir tmp/mrpc/   