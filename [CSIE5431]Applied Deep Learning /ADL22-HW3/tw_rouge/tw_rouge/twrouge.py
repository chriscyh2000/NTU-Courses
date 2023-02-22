import os

from ckiptagger import WS, data_utils
from rouge import Rouge

cache_dir = os.environ.get("XDG_CACHE_HOME", os.path.join(os.getenv("HOME"), ".cache"))
download_dir = os.path.join(cache_dir, "ckiptagger")
data_dir = os.path.join(cache_dir, "ckiptagger/data")
os.makedirs(download_dir, exist_ok=True)
if not os.path.exists(os.path.join(data_dir, "model_ws")):
    data_utils.download_data_gdown(download_dir)

ws = WS(data_dir)


def tokenize_and_join(sentences):
    return [" ".join(toks) for toks in ws(sentences)]


rouge = Rouge()


def get_rouge(preds, refs, avg=True, ignore_empty=False):
    """wrapper around: from rouge import Rouge
    Args:
        preds: string or list of strings
        refs: string or list of strings
        avg: bool, return the average metrics if set to True
        ignore_empty: bool, ignore empty pairs if set to True
    """
    if not isinstance(preds, list):
        preds = [preds]
    if not isinstance(refs, list):
        refs = [refs]
    preds, refs = tokenize_and_join(preds), tokenize_and_join(refs)
    return rouge.get_scores(preds, refs, avg=avg, ignore_empty=ignore_empty)
