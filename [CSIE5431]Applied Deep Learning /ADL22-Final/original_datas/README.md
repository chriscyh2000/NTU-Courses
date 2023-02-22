# Hahow資料集

## Objective

找出課程與課程之間關聯，預測用戶會購買的課程與有興趣的課程類別。

## About Dataset

在 Hahow 站上 2021/1/1 - 2021/12/31 一整年的「用戶」、「課程」兩維度組成的資料集。

提供的時間維度，皆以台北當地時間為主。依照各維度拆分以下 8 張資料表：

### 用戶基本資料（users.csv）

- user_id：用戶識別 ID
- gender：用戶性別
- occupation_titles：用戶職業類別。為複選，使用逗號分隔多項
- interests：用戶興趣。為複選，格式為 {分類}_{子分類}，使用逗號分隔多項
- recreation_names：用戶喜好。為複選，使用逗號分隔多項

### 課程購買記錄（篩選購買時間：2021/1/1 - 2021/12/31）
|          | train.csv | val_seen.csv | val_unseen.csv | test_seen.csv | test_unseen.csv |
| :--------: | :--------: | :--------: | :--------: | :--------: | :--------: |
| Time     | 2021/01/01 - 2021/08/31 | 2021/09/01 - 2021/10/31 | 2021/09/01 - 2021/10/31 | 2021/11/01 - 2021/12/31 | 2021/11/01 - 2021/12/31 |
| User count    | 59737 | 7748 | 11622 | 7205 | 11097 |

- user_id：用戶識別 ID
- course_id：課程識別 ID，使用空格分隔多項

### 用戶購買課程子分類（train_group.csv, val_seen_group.csv, val_unseen_group.csv）
用戶購買所有課程的子類別之集合，某一課程的子分類可以由courses.csv中的sub_groups欄位得知。

- user_id：用戶識別 ID
- subgroup：子類別識別 ID，使用空格分隔多項；類別名稱與 ID 對應關係請參照subgroups.csv

### 課程基本資料（courses.csv）

- course_id：課程識別 ID
- course_name：課程名稱
- course_price：課程價格
- teacher_id：老師識別 ID
- teacher_intro：老師簡介
- groups：課程分類
- sub_groups：課程子分類，使用逗號分隔多項
- topics：課程主題
- course_published_at_local：該課程識別 ID 的發行時間
- description：課程詳情
- will_learn：課程詳情 — 你可以學到
- required_tools：課程詳情 — 需要準備的工具 / 軟體
- recommended_background：課程詳情 — 需要具備的背景知識
- target_group：課程詳情 — 哪些人適合這堂課

### 課程章節單元內容（course_chapter_items.csv）

- course_id：課程識別 ID
- chapter_id：章節識別 ID
- chapter_no：章節編號
- chapter_name：章節名稱
- chapter_item_id：單元識別 ID
- chapter_item_no：單元編號
- chapter_item_name：單元名稱
- chapter_item_type：單元類型（課程/作業）
- video_length_in_seconds：單元影片長度（秒）

### 課程子類別編號（subgroups.csv）

- subgroup_id: 課程子類別編號
- subgroup_name: 課程子類別名稱