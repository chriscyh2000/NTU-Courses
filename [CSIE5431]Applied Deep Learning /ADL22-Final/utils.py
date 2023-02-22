import pickle
import json
import torch
import numpy as np


def get_dataset(data_path, embed_path):
    with open(embed_path, "rb") as f:
        train_users_embeddings = pickle.load(f)
    with open(data_path, "r") as f:
        train_users = json.load(f)
    dataset = []
    for i, user in enumerate(train_users):
        courses = user["labels"]
        embed = train_users_embeddings[i]
        dataset.append({"input_ids": embed, "attention_mask": [1] * len(embed), "labels": courses})
    return dataset

def pad_to(dataset, length=None):
    if length is None:
        length = max([len(x) for x in dataset])
    
    for i, x in enumerate(dataset):
        dataset[i] = np.pad(x, ((0, length - x.shape[0]), (0, 0)), "constant")
    
    return torch.tensor(dataset)


def apk(actual, predicted, k=50):
    """
    Computes the average precision at k.

    This function computes the average prescision at k between two lists of
    items.

    Parameters
    ----------
    actual : list
             A list of elements that are to be predicted (order doesn't matter)
    predicted : list
                A list of predicted elements (order does matter)
    k : int, optional
        The maximum number of predicted elements

    Returns
    -------
    score : double
            The average precision at k over the input lists

    """
    if len(predicted)>k:
        predicted = predicted[:k]

    score = 0.0
    num_hits = 0.0

    for i,p in enumerate(predicted):
        if p in actual and p not in predicted[:i]:
            num_hits += 1.0
            score += num_hits / (i+1.0)

    if not actual:
        return 0.0

    return score / min(len(actual), k)

def mapk(actual, predicted, k=50):
    """
    Computes the mean average precision at k.

    This function computes the mean average prescision at k between two lists
    of lists of items.

    Parameters
    ----------
    actual : list
             A list of lists of elements that are to be predicted 
             (order doesn't matter in the lists)
    predicted : list
                A list of lists of predicted elements
                (order matters in the lists)
    k : int, optional
        The maximum number of predicted elements

    Returns
    -------
    score : double
            The mean average precision at k over the input lists

    """
    return np.mean([apk(a,p,k) for a,p in zip(actual, predicted)])


def knn(value, features, k=5, discard=[], threshold=100):
    ### some entries in value are masked, use knn to find the nearest neighbor
    ### and use the neighbor's value to fill the masked entry
    ### value: (732, )
    ### features: (732, 512)
    ### return: (732, )
    labelscount = json.load(open("processed_datas/train_labels_count.json"))
    masked = np.where((np.array(labelscount) <= threshold))[0]
    if masked.sum() == 0:
        return value

    unmasked_value = value[:, ~masked]
    unmasked_features = features[~masked]
    return_value = value.copy()
    for m in masked:
        if m not in discard:
            dist = np.linalg.norm(unmasked_features - features[m], axis=1)
            nearest = np.argsort(dist)[:k]
            return_value[:, m] = (unmasked_value[:, nearest].mean(axis=1)*(threshold-labelscount[m]) + return_value[:, m]*labelscount[m])/threshold
    return return_value