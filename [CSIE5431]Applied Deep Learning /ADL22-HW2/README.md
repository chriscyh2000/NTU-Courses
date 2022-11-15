# NTU CSIE ADL2022 Homework 2

## Environment
```shell
# If you have conda, I recommend you to build a conda environment called "adl-hw2"
conda create --name adl-hw2 python=3.8
conda activate adl-hw2
pip install -r requirements.txt
```

## Context Selection
You can modify the data path or the pretrained model in `run_mc.sh` by yourself. The argument for `--pretrained_model_name_or_path` and `--tokenizer_name_or_path` can be changed to model repo names on hugging face.
```shell
bash run_mc.sh
```

## Question Answering
You can modify the data path or the pretrained model in `run_qa.sh` by yourself. The argument for `--pretrained_model_name_or_path` and `--tokenizer_name_or_path` can be changed to model repo names on hugging face.
```shell
bash run_qa.sh
```

## Reproduce the Prediction Result
```shell
bash ./run.sh /path/to/context.json /path/to/test.json  /path/to/pred/prediction.csv
```