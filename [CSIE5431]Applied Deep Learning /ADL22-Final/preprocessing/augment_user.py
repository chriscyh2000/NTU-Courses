from argparse import ArgumentParser, Namespace
from pathlib import Path
import csv
import json
from tqdm import tqdm
from copy import deepcopy
from random import *
def parse_args() -> Namespace:
	parser = ArgumentParser()
	parser.add_argument(
		"--train_file",
		type=Path,
		default="../original_datas/train.csv"
	)
	parser.add_argument(
		"--train_group_file",
		type=Path,
		default="../original_datas/train_group.csv"
	)
	parser.add_argument(
		"--user_file",
		type=Path,
		default="../original_datas/users.csv"
	)

	parser.add_argument(
		"--aug_train_file",
		type=Path,
		default="../processed_datas/aug_train.csv"
	)
	parser.add_argument(
		"--aug_train_group_file",
		type=Path,
		default="../processed_datas/aug_train_group.csv"
	)
	parser.add_argument(
		"--aug_user_file",
		type=Path,
		default="../processed_datas/aug_users.csv"
	)
	args = parser.parse_args()
	return args

# randomly mask 10% [interests] to aug users
def main(args: Namespace): 
	aug = 0
	train_dic={}
	train_group_dic = {}

	with open(args.train_file, 'r') as file:
		train_reader = csv.reader(file)
		next(train_reader)
		for data in tqdm(train_reader):
			train_dic[data[0]] = data[1]

	with open(args.train_group_file, 'r') as file:
		train_reader = csv.reader(file)
		next(train_reader)
		for data in tqdm(train_reader):
			train_group_dic[data[0]] = data[1]

	with open(args.user_file, 'r') as file:
		with open(args.aug_user_file, 'w') as wfile:
			tfile = open(args.aug_train_file, 'w')
			tgfile = open(args.aug_train_group_file, 'w')

			user_reader = csv.reader(file)
			user_writer = csv.writer(wfile)
			train_writer = csv.writer(tfile)
			traing_writer = csv.writer(tgfile)

			user_writer.writerow("user_id,gender,occupation_titles,interests,recreation_names".split(","))
			train_writer.writerow(["user_id","course_id"])
			traing_writer.writerow(["user_id","subgroup"])

			# skip the headers       
			next(user_reader)
			for data in tqdm(user_reader):
				to_mask = len(data[3].split(',')) // 10
				ori_id = data[0]
				aug_gen = data[1]
				aug_title = data[2]
				aug_recre = data[4]
				aug_ori_inte = data[3].split(",")
				aug += 2**to_mask
				for i in range(1,2**to_mask):
					aug_id = str(hex(int(ori_id,base=16)+i*100))[2:]
					aug_inte = set(deepcopy(aug_ori_inte))
					# print(randint(1,to_mask))
					mask = choices(list(range(len(aug_inte))),k=randint(1,to_mask))
					# print(sorted(mask))
					masked_inte = set([list(aug_inte)[i] for i in mask])
					aug_inte = list(aug_inte - masked_inte)
					user_writer.writerow([aug_id,aug_gen,aug_title,','.join(aug_inte),aug_recre])
					if ori_id in train_dic:
						train_writer.writerow([aug_id,train_dic[ori_id]])
					if ori_id in train_group_dic:
						traing_writer.writerow([aug_id,train_group_dic[ori_id]])

			tfile.close()
			tgfile.close()
	
	print(aug)
	exit()


if __name__ == "__main__":
	args = parse_args()
	# args.processed_file.parent.resolve().mkdir(exist_ok=True, parents=True)
	main(args)