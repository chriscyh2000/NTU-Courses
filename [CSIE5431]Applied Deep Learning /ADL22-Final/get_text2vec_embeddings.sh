if [ -d "$1" ]
then
    python get_text2vec_embeddings.py --data_path reproduce/processed_datas/train_courses.json &
	python get_text2vec_embeddings.py --data_path reproduce/processed_datas/test_seen_users.json &
	python get_text2vec_embeddings.py --data_path reproduce/processed_datas/test_unseen_users.json 
else
    python get_text2vec_embeddings.py --data_path processed_datas/train_courses.json &
	python get_text2vec_embeddings.py --data_path processed_datas/test_seen_users.json &
	python get_text2vec_embeddings.py --data_path processed_datas/test_unseen_users.json 
fi
