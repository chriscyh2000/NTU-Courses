python augment_user.py --aug_train_file ../reproduce/processed_datas/aug_train.csv --aug_train_group_file ../reproduce/processed_datas/aug_train_group.csv --aug_user_file ../reproduce/processed_datas/aug_users.csv
python preprocess_user.py --processed_file ../reproduce/processed_datas/combined_train_users.json --extra_labels_file ../reproduce/processed_datas/aug_train.csv --extra_data_file ../reproduce/processed_datas/aug_users.csv
python preprocess_user.py --processed_file ../reproduce/processed_datas/test_unseen_users.json  --labels_file ../original_datas/test_unseen.csv --cluster_interest
python preprocess_user.py --processed_file ../reproduce/processed_datas/test_seen_users.json  --labels_file ../original_datas/test_seen.csv --cluster_interest
python preprocess_user.py --processed_file ../reproduce/processed_datas/val_unseen_users.json  --labels_file ../original_datas/val_unseen.csv --cluster_interest
python preprocess_user.py --processed_file ../reproduce/processed_datas/val_seen_users.json  --labels_file ../original_datas/val_seen.csv --cluster_interest
python preprocess_course.py --processed_file ../reproduce/processed_datas/train_courses.json --extra_labels_file ../reproduce/processed_datas/aug_train.csv

