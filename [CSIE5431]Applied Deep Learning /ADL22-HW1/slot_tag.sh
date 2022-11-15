# "${1}" is the first argument passed to the script
# "${2}" is the second argument passed to the script
python3 test_slot.py --test_file "${1}" --ckpt_path ./ckpt/slot/model.ckpt --pred_file "${2}"