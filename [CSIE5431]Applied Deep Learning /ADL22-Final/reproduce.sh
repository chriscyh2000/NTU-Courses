### Preprocess
cd ./preprocessing
bash preprocess.sh
cd ..

### Produce embedding in 2 ways
cp reproduce/processed_datas/combined_train_users.json reproduce/processed_datas/train_users.json
bash get_text2vec_embeddings.sh ./reproduce
bash get_raw_embeddings.sh ./reproduce


### Predict for (un)seen topic id

python eval_group.py
python test_val_2.py --test_user_data reproduce/processed_datas/test_seen_users.json --course_data_path reproduce/processed_datas/train_courses.json --ckpt_path ./ckpts/model.ckpt --pred_file reproduce/results/seen_results.csv
python test_val_2.py --test_user_data reproduce/processed_datas/test_unseen_users.json --course_data_path reproduce/processed_datas/train_courses.json --ckpt_path ./ckpts/model.ckpt --pred_file reproduce/results/unseen_results.csv
