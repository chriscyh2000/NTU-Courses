#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#define HASHSIZE 25000000
#define ARRAYSIZE 200001
typedef struct node{
    int idx;
    int record;
    int vertice1;
    int vertice2;
    struct node *ptr1;
    struct node *ptr2;
    struct node *next;
}Node;
Node node[ARRAYSIZE];
Node *sortedArray[ARRAYSIZE];
Node *hashTable[HASHSIZE] = {0};
Node *tail[HASHSIZE];
int nextNode[ARRAYSIZE];
void printNode(Node *n){
    printf("%d %d\n", n->vertice1, n->vertice2);
    return;
}
int hash(int value1, int value2){
    double ratio = (sqrt(5.0) - 1) / 2;
    int h = (int)(((value2 * value1 * ratio) - (int)(value2 * value1 * ratio)) * HASHSIZE);
    if(h < 0) return (h * (-1));
    return h;
}
int max(int value1, int value2){
    return (value1 > value2) ? value1 : value2;
}
int min(int value1, int value2){
    return (value1 <= value2) ? value1 : value2;
}
Node *find(int value1, int value2, int h){
    if(hashTable[h] == 0) return NULL;
    Node *cur = hashTable[h];
    while(cur != NULL){
        if(cur->vertice1 == value1 && cur->vertice2 == value2)
            return cur;
        cur = cur->next;
    }
    return NULL;
}
void insert(Node *toInsert, int hashValue){
    if(hashTable[hashValue] == 0){
        hashTable[hashValue] = toInsert;
        tail[hashValue] = toInsert;
        return;
    }
    tail[hashValue]->next = toInsert;
    tail[hashValue] = toInsert;
    return;
}
bool topologicalSort(int nodeNum){
    if(nodeNum == 0) return true;
    int entry = -1;
    int temp;
    int i;
    for(i = 0; i < nodeNum; ++i){
        if(node[i].record == 0){
            if(entry == -1){
                entry = i;
                temp = i;
            }
            nextNode[temp] = i;
            temp = i;
            nextNode[temp] = i;
        }
    }
    int j = 0;
    if(entry == -1) return false;
    int cur = entry;
    for(int i = 0; i < nodeNum; ++i){
        if(node[cur].ptr1 != NULL){
            --node[cur].ptr1->record;
            if(node[cur].ptr1->record == 0){
                nextNode[temp] = node[cur].ptr1->idx;
                temp = node[cur].ptr1->idx;
                nextNode[temp] = temp;
            }
        }
        if(node[cur].ptr2 != NULL){
            --node[cur].ptr2->record;
            if(node[cur].ptr2->record == 0){
                nextNode[temp] = node[cur].ptr2->idx;
                temp = node[cur].ptr2->idx;
                nextNode[temp] = temp;
            }
        }
        if(nextNode[cur] == cur && i != nodeNum - 1) return false;
        sortedArray[j++] = &node[cur];
        cur = nextNode[cur];
    }
    return true;
}
int main(void){
    int totalIndex;
    scanf("%d", &totalIndex);
    int num;
    int nodeNum = 0;
    int value1, value2, h;
    int input;
    bool check;
    Node *cur, *prev;
    for(int i = 1; i <= totalIndex; ++i){
        scanf("%d", &num);
        cur = NULL;
        for(int j = 0; j < num; ++j){
            scanf("%d", &input);
            value1 = min(i, input);
            value2 = max(i, input);
            h = hash(value1, value2);
            prev = cur;
            cur = find(value1, value2, h);
            if(cur == NULL){
                node[nodeNum].idx = nodeNum;
                node[nodeNum].record = 0;
                node[nodeNum].next = NULL;
                node[nodeNum].ptr1 = NULL;
                node[nodeNum].ptr2 = NULL;
                node[nodeNum].vertice1 = value1;
                node[nodeNum].vertice2 = value2;
                insert(&node[nodeNum], h);
                cur = &node[nodeNum];
                ++nodeNum;
            }
            if(prev != NULL){
                if(prev->ptr1 == NULL)
                    prev->ptr1 = cur;
                else prev->ptr2 = cur;
                ++cur->record;      
            }
        }
    }
    if(topologicalSort(nodeNum)){
        printf("Yes\n");
        for(int i = 0; i < nodeNum; ++i)
            printNode(sortedArray[i]);
    }
    else printf("No\n");
    return 0;
}