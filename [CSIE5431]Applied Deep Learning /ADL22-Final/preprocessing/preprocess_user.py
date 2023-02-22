from argparse import ArgumentParser, Namespace
from pathlib import Path
import csv
import json

def parse_args() -> Namespace:
	parser = ArgumentParser()
	parser.add_argument(
		"--labels_file",
		type=Path,
		default="../original_datas/train.csv"
	)
	parser.add_argument(
		"--data_file",
		type=Path,
		default="../original_datas/users.csv"
	)
	parser.add_argument(
		"--sub_file",
		type=Path,
		default="../original_datas/subgroups.csv"
	)
	parser.add_argument(
		"--course_file",
		type=Path,
		default="../original_datas/courses.csv"
	)
	parser.add_argument(
		"--processed_file",
		type=Path,
		default="../processed_datas/combined_train_users.json"
	)
	parser.add_argument(
		"--data_aug",
		action="store_true",
		help="If you want to augment data",
	)
	parser.add_argument(
		"--cluster_interest",
		action="store_true",
		help="If you want to cluster_interest",
	)
	parser.add_argument(
		"--extra_labels_file",
		type=Path,
		default="../processed_datas/aug_train.csv"
	)
	parser.add_argument(
		"--extra_data_file",
		type=Path,
		default="../processed_datas/aug_users.csv"
	)
	

	
	args = parser.parse_args()
	return args


def main(args: Namespace): 
	uid2features = {}
	output_data = [] 
	course2int = {}
	sub2id={}
	course2sub={}
 
	# Read Course_tokenizer 
	with open("../course_tokenizer.json", 'r') as file:
		all_datas = json.load(file)
		all_tokens = all_datas["added_tokens"] 
		for ctoken in all_tokens:
			course2int[ctoken["content"]] = ctoken["id"]

	# Read the user features
	all_interest = dict()
	with open(args.data_file, 'r') as file:
		feature_reader = csv.reader(file)
		# skip the headers       
		next(feature_reader)
		for data in feature_reader:
			u_id = data[0]
			# sort data[3]
			if(args.cluster_interest and data[3]):
				data[3] = ','.join(sorted(data[3].split(',')))

			uid2features[u_id] = {
				'gender' : data[1],
				'occupation' : data[2],
				'interests' : data[3], 
				'recreation' : data[4]
			}
	
	# Read sub <-> id table
	with open(args.sub_file, 'r') as file:
		feature_reader = csv.reader(file)
		# skip the headers       
		next(feature_reader)
		for data in feature_reader:
			sub2id[data[1]] = int(data[0])

	
	with open(args.course_file, 'r') as file:
		feature_reader = csv.reader(file)
		# skip the headers       
		next(feature_reader)	
		for data in feature_reader:
			course2sub[data[0]] = data[6].split(",")	


	# Read the AUGMENTED user features
	if args.data_aug:
		with open(args.extra_data_file, 'r') as file:
			feature_reader = csv.reader(file)
			# skip the headers       
			next(feature_reader)
			for data in feature_reader:
				u_id = data[0]
				if(u_id in uid2features):
					raise "duplicated user uid"
				if(args.cluster_interest and data[3]):
					data[3] = ','.join(sorted(data[3].split(',')))
				uid2features[u_id] = {
					'gender' : data[1],
					'occupation' : data[2],
					'interests' : data[3], 
					'recreation' : data[4]
				} 

	# Read the training data 
	with open(args.labels_file, 'r') as file:
		data_reader = csv.reader(file)
		next(data_reader, None)                  # skip the headers
		for u_id, course_id in data_reader:
			output_text = "[性別]" + ("男性" if uid2features[u_id]['gender'] == "male" else "女性") 
			if uid2features[u_id]['occupation']:
				output_text += "[職業]" + uid2features[u_id]['occupation']
			if uid2features[u_id]['interests']:
				output_text += "[興趣]" + uid2features[u_id]['interests'] 
			if uid2features[u_id]['recreation']:
				output_text += "[娛樂]" + uid2features[u_id]['recreation']

			id_of_courses = course_id.split()
			courseids = [course2int[cid] for cid in id_of_courses]  
			subgroups = set()
			for cid in id_of_courses:
				for sub in course2sub[cid]:
					if sub:
						subgroups.add(sub2id[sub])

			output_data.append({
				"id": u_id,
				"text": output_text ,
				"labels": courseids,
				"subgroups": list(subgroups)
			}) 
   
	#Read the AUGMENTED training data
	if args.data_aug:
		with open(args.extra_labels_file, 'r') as file:
			data_reader = csv.reader(file)
			next(data_reader, None)                  # skip the headers
			for u_id, course_id in data_reader:
				output_text = (
					"[性別]" + ("男性" if uid2features[u_id]['gender'] == "male" else "女性") + 
					"[職業]" + uid2features[u_id]['occupation'] + 
					"[興趣]" + uid2features[u_id]['interests'] + 
					"[娛樂]" + uid2features[u_id]['recreation']
				)

				id_of_courses = course_id.split()
				courseids = [course2int[cid] for cid in id_of_courses]  
				subgroups = set()
				for cid in id_of_courses:
					for sub in course2sub[cid]:
						if sub:
							subgroups.add(sub2id[sub])

				output_data.append({
					"id": u_id,
					"text": output_text ,
					"labels": courseids  ,
					"subgroups": list(subgroups)
				}) 
	
	with open(args.processed_file, "w", encoding="utf8") as outjson:
		json.dump(output_data, outjson, ensure_ascii=False, indent=4)


if __name__ == "__main__":
	args = parse_args()
	args.processed_file.parent.resolve().mkdir(exist_ok=True, parents=True)
	main(args)