import gym

from gym import Env
from gym.spaces import Discrete, Box

import pickle
import json
from average_precision import apk
import random
import numpy as np

user_embed_dir = "../processed_datas/train_users_embeddings.pkl"
course_embed_dir = "../processed_datas/train_courses_embeddings.pkl"
label_dir = "../processed_datas/train_users.json"

with open(user_embed_dir, 'rb') as f:
    user_embeddings = pickle.load(f)
with open(course_embed_dir, 'rb') as f:
    course_embeddings = pickle.load(f)
with open(label_dir, 'r') as f:
    datas = json.load(f)
    user_labels = [data['labels'] for data in datas]


class RecommendCourseEnv(Env):
    def __init__(self):
        self.action_space = Discrete(732)
        "observation: user embedding" 
        self.observation_space = Box(low=-100, high=100, shape=(512, 512), dtype=np.float32)
        self.user_embeddings = user_embeddings
        self.course_embeddings = [np.mean(course_embedding, axis=0) for course_embedding in course_embeddings]
        self.user_labels = user_labels
        self.last_score = 0
        self.last_actions = []
        self.idx = random.randint(0, len(self.user_embeddings) - 1)

    def step(self, action):
        self.last_actions += [action]
        score = apk(self.user_labels[self.idx], self.last_actions, k=50)
        reward = score - self.last_score
        self.last_score = score

        self.obs["course_embed"].append(self.course_embeddings[action])

        if len(self.last_actions) >= 50 or set(self.user_labels[self.idx]).issubset(set(self.last_actions)):
            return self.obs, reward, True, {}
        return self.obs, reward, False, {}

    def reset(self, seed=None):
        self.last_score = 0
        self.last_actions = []
        if seed is not None:
            self.idx = seed % len(self.user_embeddings)
        else:
            self.idx = random.randint(0, len(self.user_embeddings) - 1)
        self.obs = {
            'user_embed': self.user_embeddings[self.idx],
            'course_embed': [self.course_embeddings[0]],
        }
        return self.obs

class RecommendCourseEnv_simp(Env):
    def __init__(self):
        self.action_space = Discrete(732)
        "observation: user embedding" 
        self.observation_space = Box(low=-100, high=100, shape=(1244, ), dtype=np.float32)
        self.user_embeddings = [np.mean(user_embeddings, axis=0) for user_embeddings in user_embeddings]
        # self.course_embeddings = [np.mean(course_embedding, axis=0) for course_embedding in course_embeddings]
        self.user_labels = user_labels
        self.last_score = 0
        self.last_actions = []
        self.idx = random.randint(0, len(self.user_embeddings) - 1)

    def step(self, action):
        self.last_actions += [action]
        score = apk(self.user_labels[self.idx], self.last_actions, k=50)
        reward = score - self.last_score
        self.last_score = score

        self.last_actions_onehot[action] = 1

        self.obs = np.concatenate([self.user_embeddings[self.idx], self.last_actions_onehot])

        if len(self.last_actions) >= 50 or set(self.user_labels[self.idx]).issubset(set(self.last_actions)):
            return self.obs, reward, True, {}
        return self.obs, reward, False, {}

    def reset(self, seed=None):
        self.last_score = 0
        self.last_actions = []
        if seed is not None:
            self.idx = seed % len(self.user_embeddings)
        else:
            self.idx = random.randint(0, len(self.user_embeddings) - 1)
        self.last_actions_onehot = np.zeros(732)
        self.obs = np.concatenate([self.user_embeddings[self.idx], self.last_actions_onehot])
        return self.obs