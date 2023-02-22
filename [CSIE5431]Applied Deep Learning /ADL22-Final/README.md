### ADL-final project  

[discussion](https://hackmd.io/hTvJNtMJQgi5-T_vMUpLIg?both)  


## Usage  

train_seq.py: train for seq2seq  
train_val.py: train for value ranking  


## Note  

For seq2seq models, I've only implemented teacher-forcing training so far.   

## How to generate text2vec embedding for course prediction
```shell
cd processed_datas
python preprocess_course.py
python preprocess_user.py --data_file ../original_datas/users.csv --processed_file ../processed_datas/train_users.json
python preprocess_user.py --data_file ../original_datas/val_unseen.csv --processed_file ../processed_datas/val_unseen_users.json
python preprocess_user.py --data_file ../original_datas/val_seen.csv --processed_file ../processed_datas/val_seen_users.json
python preprocess_user.py --data_file ../original_datas/test_unseen.csv --processed_file ../processed_datas/test_unseen_users.json
python preprocess_user.py --data_file ../original_datas/test_seen.csv --processed_file ../processed_datas/test_seen_users.json
cd ..
python get_text2vec_embeddings.py --data_path processed_datas/train_courses.json
python get_text2vec_embeddings.py --data_path processed_datas/train_users.json
python get_text2vec_embeddings.py --data_path processed_datas/val_unseen_users.json
python get_text2vec_embeddings.py --data_path processed_datas/val_seen_users.json
python get_text2vec_embeddings.py --data_path processed_datas/test_unseen_users.json
python get_text2vec_embeddings.py --data_path processed_datas/test_seen_users.json
```

## How to reproduce the result of course prediction
```shell
python courses_similarity.py --top_k 6
# for unseen users
python train_val_2.py --train_users_embedding ./processed_datas/train_users_text2vec.pkl --train_courses_embedding ./processed_datas/train_courses_text2vec.pkl --val_users_embedding ./processed_datas/val_unseen_users_text2vec.pkl --val_user_data processed_datas/val_unseen_users.json --add_unseen_courses --ckpt_dir ./ckpts/attn_unseen_val

python test_val_2.py --test_users_embedding ./processed_datas/test_unseen_users_text2vec.pkl --test_user_data processed_datas/test_unseen_users.json --ckpt_path ./ckpts/attn_unseen_val/model.ckpt

# for seen users
python train_val_2.py --train_users_embedding ./processed_datas/train_users_text2vec.pkl --train_courses_embedding ./processed_datas/train_courses_text2vec.pkl --val_users_embedding ./processed_datas/val_seen_users_text2vec.pkl --val_user_data processed_datas/val_seen_users.json --add_unseen_courses --ckpt_dir ./ckpts/attn_seen_val --users_seen

python test_val_2.py --test_users_embedding ./processed_datas/test_seen_users_text2vec.pkl --test_user_data processed_datas/test_seen_users.json --ckpt_path ./ckpts/attn_seen_val/model.ckpt --users_seen
```