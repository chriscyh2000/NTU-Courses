# NTU CSIE ADL2022 Homework 3

## Environment
```shell
# If you have conda, I recommend you to build a conda environment called "adl-hw2"
conda create --name adl-hw3 python=3.8
conda activate adl-hw3
pip install -r requirements.txt
```

## Download Trained Model and Data
```
bash ./download.sh
```

## Reproduce the Prediction Result
```shell
bash ./run.sh /path/to/input.jsonl  /path/to/pred/output.jsonl
```