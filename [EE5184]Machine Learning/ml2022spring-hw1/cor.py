'''to get the corr between third day positive and other feature which are bigger than 0.8'''
import pandas as pd
data = pd.read_csv("~/Desktop/NTU110/NTU1102/ml1102/ml2022spring-hw1/covid.train.csv", header = None).iloc[:, 1:]
cor = data.iloc[1:, :].astype(float)
list0 = list(cor.corr().iloc[:, -1])

feat = list(data.iloc[0, :])

list1 = []
i = 0
for feature in feat:
	list1.append([feature, i])
	i += 1
i = 0
for val in list0:
	list1[i].append(val)
	i += 1
ans = []
for row in list1:
	if(abs(row[2]) >= 0.8 and row[1] != 116):
		ans.append(row[1])
print(ans, end = "\n")
# for i in range(5):
#     print(i)