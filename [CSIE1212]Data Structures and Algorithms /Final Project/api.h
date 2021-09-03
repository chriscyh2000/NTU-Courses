// this is only the testing version of api.h
// the input format and hashing function will be
// different on DSA Judge

#ifndef API_H
#define API_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

typedef struct mail {
	int id;
	char from[32], to[32];
	char subject[256], content[100000];
} mail;

typedef struct query {
	int id;
	double reward;

	enum query_type {
		expression_match,
		find_similar,
		group_analyse
	} type;

	union query_data {
		struct {
			char expression[2048];
		} expression_match_data;

		struct {
			int mid;
			double threshold;
		} find_similar_data;

		struct {
			int len, mids[512];
		} group_analyse_data;
	} data;
} query;

long _get_process_cputime(){
  static struct timespec t;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
  return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

void _assert(bool condition, char message[]){
	if(!condition){
		printf("assertion failed: %s", message);
		dup(0);  // calling a forbidden syscall to trigger Security Error on DSA Judge
		exit(1);
	}
}

void _answer(int qid, int answers[], int len){
	static bool initialized = false;
	static long _begin = 0;

	if(initialized == false){
		initialized = true;
		_begin = _get_process_cputime();
		return;
	}

	if(_get_process_cputime() - 10000 > _begin)
		exit(0);

	unsigned long long digest = 0x76efa04b78375b4b;
	for(int i = 0; i < len; i++){
		digest = (digest >> 17) | (digest << (64 - 17));
    	digest ^= (unsigned long long)answers[i];
    	digest = ~digest;
	}
	printf("%d %llx\n", qid, digest);
}

void api_init(int *n_mails, int *n_queries, mail **mails, query **queries){
	_assert(_get_process_cputime() < 15, "Calling init too late");

	// reading mails
	char buffer[102400];
	fgets(buffer, 1024, stdin);
	sscanf(buffer, "%d", n_mails);

	*mails = (mail *)malloc(*n_mails * sizeof(mail));

	fgets(buffer, 1024, stdin);
	_assert(strcmp(buffer, "=== dc685a9c7684059f ===\n") == 0, "Wrong input format");

	for(int i = 0; i < *n_mails; i++){
		mail *m = &(*mails)[i];

		fgets(buffer, 1024, stdin);
		sscanf(buffer, "%d", &(m->id));


		fgets(m->from, 32, stdin);
		m->from[strlen(m->from) - 1] = '\0';


		fgets(m->content, 100000, stdin);
		m->content[strlen(m->content) - 1] = '\0';

		fgets(m->subject, 256, stdin);
		m->subject[strlen(m->subject) - 1] = '\0';

		fgets(m->to, 32, stdin);
		m->to[strlen(m->to) - 1] = '\0';

		fgets(buffer, 1024, stdin);
		_assert(strcmp(buffer, "=== e47bbae5876702dc ===\n") == 0, "Wrong input format");
	}

	// reading queries
	fgets(buffer, 1024, stdin);
	sscanf(buffer, "%d", n_queries);

	*queries = (query *)malloc(*n_queries * sizeof(query));

	fgets(buffer, 1024, stdin);
	_assert(strcmp(buffer, "=== dc685a9c7684059f ===\n") == 0, "Wrong input format");

	for(int i = 0; i < *n_queries; i++){
		query *q = &(*queries)[i];

		int len;
		unsigned long long seed;

		fgets(buffer, 1024, stdin);
		sscanf(buffer, "%d %lf", &q->id, &q->reward);

		fgets(buffer, 1024, stdin);
		switch(buffer[0]){
			case 'e':
				q->type = expression_match;
				fgets(q->data.expression_match_data.expression, 2048, stdin);
				q->data.expression_match_data.expression[strlen(q->data.expression_match_data.expression) - 1] = '\0';
				break;
			case 'f':
				q->type = find_similar;
				fgets(buffer, 1024, stdin);
				sscanf(buffer, "%d %lf", &q->data.find_similar_data.mid, &q->data.find_similar_data.threshold);
				break;
			case 'g':
				q->type = group_analyse;
				fgets(buffer, 1024, stdin);
				sscanf(buffer, "%llu %d", &seed, &len);

				for(int i = 0; i < len; i++){
					seed = (seed * 48271LL) % 2147483647;
					q->data.group_analyse_data.mids[i] = seed % (*n_mails);
				}

				q->data.group_analyse_data.len = len;

				break;
		}

		fgets(buffer, 1024, stdin);
		_assert(strcmp(buffer, "=== e47bbae5876702dc ===\n") == 0, "Wrong input format");
	}

	_answer(0, 0, 0);
}


struct {
	void (*init)(int *n_mails, int *n_queries, mail **mails, query **queries);
	void (*answer)(int qid, int answers[], int len);
} api = {api_init, _answer};

#endif
