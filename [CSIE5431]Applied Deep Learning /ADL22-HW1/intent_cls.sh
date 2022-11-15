# "${1}" is the first argument passed to the script
# "${2}" is the second argument passed to the script
python3 test_intent.py --test_file "${1}" --ckpt_path ./ckpt/intent/model.ckpt --pred_file "${2}"