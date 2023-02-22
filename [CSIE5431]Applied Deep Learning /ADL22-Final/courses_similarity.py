from argparse import ArgumentParser, Namespace
from pathlib import Path
import json
from text2vec import SentenceModel, semantic_search

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--training_user_data",
        type=Path,
        default="./processed_datas/train_users.json"
    )
    parser.add_argument(
        "--training_course_data",
        type=Path,
        default="./processed_datas/train_courses.json"
    )
    parser.add_argument(
        "--total_course",
        type=int,
        default=728
    )
    parser.add_argument(
        "--top_k",
        type=int,
        default=3
    )
    args = parser.parse_args()
    return args

def main(args):
    users = json.loads(args.training_user_data.read_text())
    courses = json.loads(args.training_course_data.read_text())
    
    embedder = SentenceModel()
    seen_course = set()
    for user in users:
        user["labels"] = [label - 4 for label in user["labels"]]
        seen_course.update(user["labels"])
    unseen_course = set([i for i in range(args.total_course)]) - seen_course
    unseen_course = sorted(list(unseen_course))
    
    queries = [course["text"] for course in courses]
    courses = [courses[i] for i in unseen_course]
    corpus = [course["text"] for course in courses]

    corpus_embedding = embedder.encode(corpus)
    
    similar_course = []
    
    for i, query in enumerate(queries):
        query_embedding = embedder.encode(query)
        hits = semantic_search(query_embedding, corpus_embedding, top_k=args.top_k)
        hits = hits[0]
        if unseen_course[hits[0]["corpus_id"]] == i: 
            hits = hits[1:]
        similar_course.append([unseen_course[hit["corpus_id"]] for hit in hits])
        
    with open("unseen_similar_courses.json", "w") as f:   
        json.dump(similar_course, f)
            
if __name__ == "__main__":
    args = parse_args()
    main(args)