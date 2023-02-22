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
		default="../original_datas/courses.csv"
	)
	parser.add_argument(
		"--extra_labels_file",
		type=Path,
		default="../processed_datas/aug_train.csv"
	)
	parser.add_argument(
		"--processed_file",
		type=Path,
		default="../processed_datas/train_courses.json"
	)
	args = parser.parse_args()
	return args
	
def main(args: Namespace): 
	id2features = {}
	output_data = [] 
	course2int = {}
 
	# Read Course_tokenizer 
	with open("../course_tokenizer.json", 'r') as file:
		all_datas = json.load(file)
		all_tokens = all_datas["added_tokens"] 
		for ctoken in all_tokens:
			course2int[ctoken["content"]] = ctoken["id"]

	"""
	course_id,course_name,course_price,teacher_id,teacher_intro,
	groups,sub_groups,topics,course_published_at_local,description,
	will_learn,required_tools,recommended_background,target_group
	"""
	# Read the courses features
	with open(args.data_file, 'r') as file:
		feature_reader = csv.reader(file)
		# skip the headers       
		next(feature_reader)
		for data in feature_reader:
			course_id = data[0]
			id2features[course_id] = {
				'course_name' : data[1],
				'course_price' : data[2],
				# 'teacher_introduction' : data[4], 
				'course_group' : data[5],
                'course_subgroup' : data[6],
                'topic' : data[7],
                # 'course_published_at_local' : data[8],
                # 'description' : data[9],
                'will_learn' : data[10],
                'required_tools' : data[11],
                'recommended_background' : data[12],
                'target_group' : data[13]
			} 

	for course_id in id2features:
		output_text = (
            "[目標客群]" + id2features[course_id]['target_group'] +
			"[課程類別]" + id2features[course_id]['course_group'] +
            "[課程子類別]" + id2features[course_id]['course_subgroup'] +
			"[課程分類]" + id2features[course_id]['topic'] 
		)
		output_data.append({
			"text": output_text.replace("\n",'') ,
			"id":course_id
		}) 
	
	with open(args.processed_file, "w", encoding="utf8") as outjson:
		json.dump(output_data, outjson, ensure_ascii=False, indent=4)


if __name__ == "__main__":
	args = parse_args()
	args.processed_file.parent.resolve().mkdir(exist_ok=True, parents=True)
	main(args)