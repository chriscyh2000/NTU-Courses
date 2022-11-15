import numpy as np
import pandas as pd

import csv

def read_csv(file_name):
    data = pd.read_csv(file_name, header=None)
    pred_arr = data.iloc[1:, 1:]
    return np.array(pred_arr, dtype = np.float64).squeeze(1)

avg = np.zeros(1078)
for i in range(3):
    file_name = 'pred' + str(i) + '.csv'
    # print(read_csv(file_name))
    tmp = read_csv(file_name)
    avg +=  tmp

avg /= 3


with open('ens_res.csv', 'w') as fp:
    fp.write('id,tested_positive\n')
    for i, p in enumerate(avg):
        fp.write('{},{}\n'.format(i, p))
