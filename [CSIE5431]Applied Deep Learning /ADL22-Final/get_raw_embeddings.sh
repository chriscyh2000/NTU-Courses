if [ -d "$1" ]
then
	python get_raw_embeddings.py --seq_maxlen 128 --data_path reproduce/processed_datas/train_courses.json &
	python get_raw_embeddings.py --seq_maxlen 128 --data_path reproduce/processed_datas/train_users.json
else
	python get_raw_embeddings.py --seq_maxlen 128 --data_path processed_datas/train_courses.json &
	python get_raw_embeddings.py --seq_maxlen 128 --data_path processed_datas/train_users.json
fi