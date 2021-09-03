#include "api.h"

int n_mails, n_queries;
mail *mails;
query *queries;

#include <ctype.h>
#define min(x,y) (x < y) ? x : y
#define MAILNUM 10000
#define TABLESIZE 2000
#define HASHSIZE 10000

typedef struct token{
	int originalKey;
	char c;
	int length;
	struct token *next;
}Token;

int ans[MAILNUM] = {0};
int size[MAILNUM] = {0};
int total_num_key[MAILNUM] = {0};
int keys[MAILNUM][TABLESIZE];
int table[MAILNUM][HASHSIZE] = {0};
Token tokens[3000000];
int count = 0;

typedef struct person{
	int parent;
	int size;
}Person;

Person people[MAILNUM];
int names[MAILNUM][2] = {0};
int keys_GA[400000] = {0};

static inline int group_analyse_hash(const char *s){
	int ret = 0;
	int l = strlen(s);
	for (int i = 0; i < l; ++i)
		ret = (ret * 26 + (s[i]=='a'? s[i] - 'a' : s[i] - 'A') + 1) % 384615;
	return ret;
}

static inline void make_set(int index){
	people[index].parent = index;
	people[index].size = 1;
	++ans[0];
	return ;
}

static inline int find_set(int index){
	if(people[index].parent != index)
		people[index].parent = find_set(people[index].parent);
	return people[index].parent;
}

static inline void set_union(int index1, int index2){
	index1 = find_set(index1), index2 = find_set(index2);
	if(index1 == index2)
		return;
	if(people[index1].size > people[index2].size){
		people[index2].parent = index1;
		people[index1].size += people[index2].size;
		ans[1] = people[index1].size > ans[1]? people[index1].size : ans[1];
	}
	else{
		people[index1].parent = index2;
		people[index2].size += people[index1].size;
		ans[1] = people[index2].size > ans[1]? people[index2].size : ans[1];
	}
	--ans[0];
	return;
}

static inline bool checkTorken(Token *t1, Token *t2){
	return (t1->length == t2->length && t1->originalKey == t2->originalKey && t1->c == t2->c);
}
static inline bool checkExistance(Token *t, int ori, int len, char c){
	return (t->length == len && t->originalKey == ori && t->c == c);
}

static inline char char_to_value(char c){
   if(islower(c))
	   return c - 87;
   else if(isupper(c))
	   return c - 55;
   else if(48 <= c && c <= 57)
	   return c - 48;
   return -1;
}


static inline void get_token(){
	int Q = HASHSIZE / 36;
	int num, index, key, len, ori;
	char c;
	for(int mid = 0; mid < n_mails; ++mid){
		mail cur = mails[mid];
		num = 0;
		index = 0;
		key = 0;
		len = 0;
		ori = 0;
		while(cur.content[index]){
			int value = char_to_value(cur.content[index]);
			if(value > -1){
				key = (key * 36 + value) % Q;
				ori += toupper(cur.content[index]);
				++len;
				++index;
			}
			else{
				if(len){
					key = (key + char_to_value(cur.content[index-1]) * char_to_value(cur.content[index-len]) + len) % HASHSIZE;
					if(!table[mid][key]){
						tokens[count].originalKey = ori;
						tokens[count].c = toupper(cur.content[index-len]);
						tokens[count].length = len;
						tokens[count].next = NULL;
						table[mid][key] = count++;
						keys[mid][num++] = key;
						++total_num_key[mid];
					}
					else{
						Token *curToken = &(tokens[table[mid][key]]), *prevToken;
						char c = toupper(cur.content[index-len]);
						while(curToken){
							if(checkExistance(curToken, ori, len, c))
								break;
							prevToken = curToken;
							curToken = curToken->next;
						}
						if(!curToken){
							tokens[count].originalKey = ori;
							tokens[count].c = c;
							tokens[count].length = len;
							tokens[count].next = NULL;
							prevToken->next = &(tokens[count++]);
							++total_num_key[mid];
						}
					}
					len = 0;
					key = 0;
					ori = 0;
				}
				++index;
				while(char_to_value(cur.content[index]) == -1 && cur.content[index])
					++index;
			}
		}
		if(len){
			key = (key + char_to_value(cur.content[index-1]) * char_to_value(cur.content[index-len]) + len) % HASHSIZE;
			if(!table[mid][key]) {
				tokens[count].originalKey = ori;
				tokens[count].c = toupper(cur.content[index-len]);
				tokens[count].length = len;
				tokens[count].next = NULL;
				table[mid][key] = count++;
				keys[mid][num++] = key;
				++total_num_key[mid];
			}
			else{
				Token *curToken = &(tokens[table[mid][key]]), *prevToken;
				c = toupper(cur.content[index-len]);
				while(curToken){

					if(checkExistance(curToken, ori, len, c))
						break;
					prevToken = curToken;
					curToken = curToken->next;
				}
				if(!curToken){
					tokens[count].originalKey = ori;
					tokens[count].c = c;
					tokens[count].length = len;
					tokens[count].next = NULL;
					prevToken->next = &(tokens[count++]);
					++total_num_key[mid];
				}
			}
		}
		key = 0;
		len = 0;
		ori = 0;
		index = 0;
		while(cur.subject[index]){
			int value = char_to_value(cur.subject[index]);
			if(value > -1){
				key = (key * 36 + value) % Q;
				ori += toupper(cur.subject[index]);
				++len;
				++index;
			}
			else{
				if(len){
					key = (key + char_to_value(cur.subject[index-1]) * char_to_value(cur.subject[index-len]) + len) % HASHSIZE;
					if(!table[mid][key]){
						tokens[count].originalKey = ori;
						tokens[count].c = toupper(cur.subject[index-len]);
						tokens[count].length = len;
						tokens[count].next = NULL;
						table[mid][key] = count++;
						keys[mid][num++] = key;
						++total_num_key[mid];
					}
					else{
						Token *curToken = &(tokens[table[mid][key]]), *prevToken;
						char c = toupper(cur.subject[index-len]);
						while(curToken){
							if(checkExistance(curToken, ori, len, c))
								break;
							prevToken = curToken;
							curToken = curToken->next;
						}
						if(!curToken){
							tokens[count].originalKey = ori;
							tokens[count].c = c;
							tokens[count].length = len;
							tokens[count].next = NULL;
							prevToken->next = &(tokens[count++]);
							++total_num_key[mid];
						}
					}
					ori = 0;
					len = 0;
					key = 0;
				}
				++index;
				while(char_to_value(cur.subject[index]) == -1 && cur.subject[index])
					++index;
			}
		}
		if(len){
			key = (key + char_to_value(cur.subject[index-1]) * char_to_value(cur.subject[index-len]) + len) % HASHSIZE;
			if(!table[mid][key]) {
				tokens[count].originalKey = ori;
				tokens[count].c = toupper(cur.subject[index-len]);
				tokens[count].length = len;
				tokens[count].next = NULL;
				table[mid][key] = count++;
				keys[mid][num++] = key;
				++total_num_key[mid];
			}
			else{
				Token *curToken = &(tokens[table[mid][key]]), *prevToken;
				char c = toupper(cur.subject[index-len]);
				while(curToken){
					if(checkExistance(curToken, ori, len, c))
						break;
					prevToken = curToken;
					curToken = curToken->next;
				}
				if(!curToken){
					tokens[count].originalKey = ori;
					tokens[count].c = c;
					tokens[count].length = len;
					tokens[count].next = NULL;
					prevToken->next = &(tokens[count++]);
					++total_num_key[mid];
				}
			}
		}
		size[mid] = num;
	}
	return ;
}

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	get_token();
	int qqq[200000];
	int qlen = 0;
	for(int i = 0; i < n_queries; ++i){
		if(queries[i].type == expression_match){
		}
		else if(queries[i].type == find_similar){
		int mid = queries[i].data.find_similar_data.mid;
		double threshold = queries[i].data.find_similar_data.threshold;
		int num = 0;
		if(size[mid] > 100)
			continue;
		
		int check;
		double stop;
		num = 0;
		mid = queries[i].data.find_similar_data.mid;
		threshold = queries[i].data.find_similar_data.threshold;
		for(int t = 0; t < mid; ++t){
			stop = (double)(total_num_key[mid] + total_num_key[t]) * threshold / (1 + threshold);
			if(stop > total_num_key[mid] || stop > total_num_key[t]) continue;
			for(int j = 0; j < size[mid]; ++j){
				if(stop + 1 < 0.999999) break;
				check = keys[mid][j];
				if(table[t][check]){
					for(Token *cur1 = &(tokens[table[mid][check]]); cur1; cur1 = cur1->next){
						for(Token *cur2 = &(tokens[table[t][check]]); cur2; cur2 = cur2->next){
							if(checkTorken(cur1, cur2)){
								--stop;
								break;
							}
						}
					}
				}
			}
			if(stop + 1 < 0.999999)
				ans[num++] = t;
		}
		for(int t = mid + 1; t < n_mails; ++t){
			stop = (double)(total_num_key[mid] + total_num_key[t]) * threshold / (1 + threshold);
			if(stop > total_num_key[mid] || stop > total_num_key[t]) continue;
			for(int j = 0; j < size[mid]; ++j){
				if(stop + 1 < 0.999999) break;
				check = keys[mid][j];
				if(table[t][check]){
					for(Token *cur1 = &(tokens[table[mid][check]]); cur1; cur1 = cur1->next){
						for(Token *cur2 = &(tokens[table[t][check]]); cur2; cur2 = cur2->next){
							if(checkTorken(cur1, cur2)){
								--stop;
								break;
							}
						}
					}
				}
			}
			if(stop + 1 < 0.999999)
				ans[num++] = t;
		}
		api.answer(queries[i].id, ans, num);
		}
		else if(queries[i].type == group_analyse){
			int len = queries[i].data.group_analyse_data.len;
			int *mids = queries[i].data.group_analyse_data.mids;
			int index = 1;
			ans[0] = 0, ans[1] = 0;
			for(int j = 0; j < len; ++j){
				int key1 = !names[mids[j]][0]? names[mids[j]][0] = group_analyse_hash(mails[mids[j]].from) : names[mids[j]][0];
				int key2 = !names[mids[j]][1]? names[mids[j]][1] = group_analyse_hash(mails[mids[j]].to) : names[mids[j]][1];
				if(key1 == key2)
					continue;
				if(!keys_GA[key1]){
					keys_GA[key1] = index;
					make_set(index++);
				}
				if(!keys_GA[key2]){
					keys_GA[key2] = index;
					make_set(index++);
				}
				set_union(keys_GA[key1], keys_GA[key2]);
			}
			for(int j = 0; j < len; ++j){
				keys_GA[names[mids[j]][0]] = 0;
				keys_GA[names[mids[j]][1]] = 0;
			}
			api.answer(queries[i].id, ans, 2);
		}
	}
  	return 0;
}