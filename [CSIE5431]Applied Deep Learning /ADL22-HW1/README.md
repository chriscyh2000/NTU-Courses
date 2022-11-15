# NTU CSIE ADL2022 Homework 1 

## Environment
```shell
# If you have conda, I recommend you to build a conda environment called "adl-hw1"
make
conda activate adl-hw1
pip install -r requirements.txt
# Otherwise
pip install -r requirements.in
```

## Download Glove Embedding `glove.840B.300d.txt`
Download `glove.840B.300d.txt` from https://www.kaggle.com/datasets/takuok/glove840b300dtxt and save it in `ADL22-HW1`.

## Preprocessing
```shell
# To preprocess intent detectiona and slot tagging datasets
bash preprocess.sh
```

## Intent detection 
```shell
python3 train_intent.py --device {YOUR_GPU} # Training 
python3 test_intent.py --test_file {TEST_FILE} --ckpt_path {CKPT_PATH} --pred_file {PRED_FILE} # Testing
```

## Slot tagging
```shell
python3 train_slot.py --device {YOUR_GPU}# Training
python3 test_slot.py --test_file {TEST_FILE} --ckpt_path {CKPT_PATH} --pred_file {PRED_FILE} # Testing
```