#include <stdio.h>

typedef struct node{
    int key;
    int leftIndex;
    int rightIndex;
}Node;

int Count(Node bTreeNode[], int enterIndex, int min, int max){
    if(enterIndex == -1) return 0;
    if(max - min < 2) return 0;
    int count = 0;
    if(bTreeNode[enterIndex].key < max && bTreeNode[enterIndex].key > min) ++count;
    if(max > bTreeNode[enterIndex].key) 
        count += Count(bTreeNode, bTreeNode[enterIndex].leftIndex, min, bTreeNode[enterIndex].key);
    else
        count += Count(bTreeNode, bTreeNode[enterIndex].leftIndex, min, max);
    if(bTreeNode[enterIndex].key > min)
        count += Count(bTreeNode, bTreeNode[enterIndex].rightIndex, bTreeNode[enterIndex].key, max);
    else
        count += Count(bTreeNode, bTreeNode[enterIndex].rightIndex, min, max);
    return count;
}

int main(void){
    int N;
    int inKey;
    int inLeft, inRight;
    scanf("%d", &N);
    Node bTreeNode[N+1];
    for(int i = 1; i < N + 1; i++){
        scanf("%d%d%d", &inKey, &inLeft, &inRight);
        bTreeNode[i].key = inKey;
        bTreeNode[i].leftIndex = inLeft;
        bTreeNode[i].rightIndex = inRight;
    }
    printf("%d\n", Count(bTreeNode, 1, 0, 1e9 + 1));
}