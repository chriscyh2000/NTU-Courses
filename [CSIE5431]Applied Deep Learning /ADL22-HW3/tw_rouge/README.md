# ROUGE score calculator with tradition chinese word segmentation

## Install

`pip install` this repo

## Usage

```
>>> from tw_rouge import get_rouge
>>> get_rouge('我是人', '我是一個人')
{'rouge-1': {'f': 0.7499999953125, 'p': 1.0, 'r': 0.6}, 'rouge-2': {'f': 0.33333332888888895, 'p': 0.5, 'r': 0.25}, 'rouge-l': {'f': 0.7499999953125, 'p': 1.0, 'r': 0.6}}
>>> get_rouge(['我是人'], [ 我是一個人'])
{'rouge-1': {'f': 0.7499999953125, 'p': 1.0, 'r': 0.6}, 'rouge-2': {'f': 0.33333332888888895, 'p': 0.5, 'r': 0.25}, 'rouge-l': {'f': 0.7499999953125, 'p': 1.0, 'r': 0.6}}
>>> get_rouge(['我是人'], ['我是一個人'], avg=False)
[{'rouge-1': {'f': 0.7499999953125, 'p': 1.0, 'r': 0.6}, 'rouge-2': {'f': 0.33333332888888895, 'p': 0.5, 'r': 0.25}, 'rouge-l': {'f': 0.7499999953125, 'p': 1.0, 'r': 0.6}}]
```
