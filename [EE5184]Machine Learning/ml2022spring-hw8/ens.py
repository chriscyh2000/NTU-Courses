import csv
import enum
import numpy as np
pred1 = np.genfromtxt('prediction-2.csv', delimiter=',', skip_header=1)[:, -1]
pred2 = np.genfromtxt('prediction-3.csv', delimiter=',', skip_header=1)[:, -1]
res = (pred1 + pred2) / 2 
with open('ens.csv', 'w', newline='\n') as f:
    f.write("ID,score\n")
    for i, s in enumerate(res):
        f.write(f"{i},{s:.6f}\n")