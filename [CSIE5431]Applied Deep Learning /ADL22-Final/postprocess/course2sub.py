from argparse import ArgumentParser, Namespace
from pathlib import Path
import csv
import json

def parse_args() -> Namespace:
	parser = ArgumentParser()
	parser.add_argument(
		"--pred_file", #要丟上去kaggle 有course_id的predict檔案
		type=Path,
	)
	parser.add_argument(
		"--course_file",
		type=Path,
		default="../original_datas/courses.csv"
	)
	parser.add_argument(
		"--sub_file",
		type=Path,
		default="../original_datas/subgroups.csv"
	)
	
	args = parser.parse_args()
	return args
	
def main(args: Namespace): 
	sub2id={}
	course2sub={}
	with open(args.sub_file, 'r') as file:
		feature_reader = csv.reader(file)
		# skip the headers       
		next(feature_reader)
		for data in feature_reader:
			sub2id[data[1]] = data[0]

	
	with open(args.course_file, 'r') as file:
		feature_reader = csv.reader(file)
		# skip the headers       
		next(feature_reader)	
		for data in feature_reader:
			course2sub[data[0]] = data[6].split(",")	
	"""
	course_id,course_name,course_price,teacher_id,teacher_intro,
	groups,sub_groups,topics,course_published_at_local,description,
	will_learn,required_tools,recommended_background,target_group
	"""
	count = 0
	with open(args.pred_file, 'r') as file:
		with open(str(args.pred_file).split("/")[-1].split(".")[-2]+"_sub.csv", 'w') as out:
			out_writer = csv.writer(out)
			out_writer.writerow(["user_id","subgroup"])
			feature_reader = csv.reader(file)

			# skip the headers       
			next(feature_reader)
			for data in feature_reader:
				subs = set()
				for cid in data[1].split():
					for sub in course2sub[cid]:
						if sub:
							subs.add(sub2id[sub])
				out_writer.writerow([data[0]," ".join(list(subs))])
				count+=1
				if(count>5):exit()
				
	
	# with open(args.processed_file, "w", encoding="utf8") as outjson:
	# 	json.dump(output_data, outjson, ensure_ascii=False, indent=4)


if __name__ == "__main__":
	args = parse_args()
	# args.processed_file.parent.resolve().mkdir(exist_ok=True, parents=True)
	main(args)