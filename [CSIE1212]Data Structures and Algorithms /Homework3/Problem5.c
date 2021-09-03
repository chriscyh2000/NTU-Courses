#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXBOOKNUM 1600000
#define INITIAL 800000
typedef struct node{
    bool reverseTag;
    int value;
    int priority;
    int cnt;
    int maxInTree;
    int addedValue;
    struct node *left;
    struct node *right;
}Node;
Node treap_node[MAXBOOKNUM];
int value[INITIAL];
int count = 0;
static inline void swap(Node **a, Node **b){
    Node *temp = *a;
    *a = *b;
    *b = temp;
}
static inline int cnt(Node *n){
    return (n) ? n->cnt : 0;
}
static inline void push(Node *n){
    if(n == NULL) return;
    if(n->reverseTag){
        n->reverseTag = false;
        swap(&(n->left), &(n->right));
        if(n->left) n->left->reverseTag = n->left->reverseTag ^ true;
        if(n->right) n->right->reverseTag = n->right->reverseTag ^ true;
    }
    if(n->addedValue != 0){
        if(n->left) n->left->addedValue += n->addedValue;
        if(n->right) n->right->addedValue += n->addedValue;
        n->value += n->addedValue;
        n->addedValue = 0;
    }
}
static inline void updateCnt(Node *n){
    if(n) n->cnt = cnt(n->left) + cnt(n->right) + 1;
}
static inline void updateMax(Node *n){ 
    if(!n) return;
    int l, m = n->value + n->addedValue, r;
    if(!(n->left) && !(n->right)){
        n->maxInTree = m;
    }
    else if(n->left && !(n->right)){
        l = n->left->maxInTree + n->addedValue + n->left->addedValue;
        n->maxInTree = (l >= m) ? l : m;
    } 
    else if(!(n->left) && n->right){
        r = n->right->maxInTree + n->addedValue + n->right->addedValue;
        n->maxInTree = (r > m) ? r : m;
    }
    else{
        l = n->left->maxInTree + n->addedValue + n->left->addedValue;
        r = n->right->maxInTree + n->addedValue + n->right->addedValue;
        int temp = (l >= r) ? l : r;  
        n->maxInTree = (temp >= m) ? temp : m;
    }
}
static inline void merge(Node **root, Node *leftNode, Node *rightNode){
    push(leftNode);
    push(rightNode);
    if((!leftNode) || (!rightNode))
        (*root) = (!leftNode) ? rightNode : leftNode;
    else if(leftNode->priority > rightNode->priority){
        merge(&(leftNode->right), leftNode->right, rightNode);
        (*root) = leftNode;
    }
    else{
        merge(&(rightNode->left), leftNode, rightNode->left);
        (*root) = rightNode;
    }
    updateCnt(*root);
    updateMax(*root);
}
static inline void split(Node *root, Node **left, Node **right, int key, int add){
    if(!root){
       (*left) = NULL;
       (*right) = NULL;
       return;
    }
    push(root);
    int curKey = cnt(root->left) + add;
    if(key > curKey){
        split(root->right, &(root->right), right, key, add + cnt(root->left) + 1);
        (*left) = root;
    }
    else{
        split(root->left, left, &(root->left), key, add);
        (*right) = root;
    }
    updateCnt(root);
    updateMax(root);
}
Node *genTreapNode(int value, int key){
    treap_node[count].value = value;
    treap_node[count].cnt = 1;
    treap_node[count].left = NULL;
    treap_node[count].right = NULL;
    treap_node[count].maxInTree = value;
    treap_node[count].priority = rand();
    treap_node[count].reverseTag = false;
    treap_node[count].addedValue = 0;
    return &treap_node[count++];
}
static inline void insert(Node **root, Node *newNode, int pos){ //operation1
    if(!(*root)) (*root) = newNode;
    else{
        Node *t1, *t2;
        split((*root), &t1, &t2, pos, 0);
        merge(&t1, t1, newNode);
        merge(root, t1, t2);
    }
    updateCnt(*root);
    updateMax(*root);
}
static inline void heapify(Node *root){
    if(!root) return;
    Node *maxNode = root;
    if(root->left != NULL && root->left->priority > maxNode->priority)
        maxNode = root->left;
    if(root->right != NULL && root->right->priority > maxNode->priority)
        maxNode = root->right;
    if(maxNode != root){
        int temp = maxNode->priority;
        maxNode->priority = root->priority;
        root->priority = temp;
        heapify(maxNode);
    }
}
static inline Node *build(int value[], int arraySize){
    if(arraySize == 0) return NULL;
    int mid = arraySize / 2;
    Node *root = genTreapNode(value[mid], mid);
    root->left = build(value, mid);
    root->right = build(&value[mid+1], arraySize - mid - 1);
    heapify(root);
    updateCnt(root);
    updateMax(root);
    return root;
}
static inline void reverse(Node **root, int l, int r){ //operation5
    Node *t1, *t2, *t3;
    split(*root, &t1, &t2, l - 1, 0);
    split(t2, &t2, &t3, r - (l - 1), 0);
    t2->reverseTag = t2->reverseTag ^ true;
    merge(root, t1, t2);
    merge(root, *root, t3);
}
static inline void delete(Node **root, int k, int add){ //operation2
    if(!(*root)) return;
    push(*root);
    int curKey = cnt((*root)->left) + add + 1;
    if(k == curKey) merge(root, (*root)->left, (*root)->right);
    else if(k > curKey) delete(&((*root)->right), k, add + cnt((*root)->left) + 1);
    else delete(&((*root)->left), k, add);
    updateCnt(*root);
    updateMax(*root);
}
static inline int query(Node **root, int l, int r){ //operation4
    Node *t1, *t2, *t3;
    split(*root, &t1, &t2, l - 1, 0);
    split(t2, &t2, &t3, r - (l - 1), 0);
    
    int ans = t2->maxInTree;
    merge(root, t1, t2);
    merge(root, *root, t3);
    return ans;
}
static inline void addition(Node **root, int l, int r, int added){ //operation3
    Node *t1, *t2, *t3;
    split(*root, &t1, &t2, l - 1, 0);
    split(t2, &t2, &t3, r - (l - 1), 0);
    t2->addedValue += added;
    updateMax(t2);
    merge(root, t1, t2);
    merge(root, *root, t3);
    updateMax(*root);
}
static inline int findMaxNode(Node *root){
    Node *cur = root;
    int max = root->maxInTree;
    int add = 0;
    push(cur);
    while(cur){
        push(cur);
        updateMax(cur);
        if(cur->left != NULL && cur->left->maxInTree + cur->left->addedValue == max)
            cur = cur->left;
        else if(max == cur->value) break;
        else{
            add += cnt(cur->left) + 1;
            cur = cur->right;  
        } 
    }
    return cnt(cur->left) + 1 + add;
}
static inline void removeLargest(Node **root){ //operation6
    if(!(*root)) return;
    delete(root, findMaxNode(*root), 0);
}
int main(void){
    int N, Q;
    scanf("%d%d", &N, &Q);
    for(int i = 0; i < N; ++i) scanf("%d", &value[i]);
    Node *root = NULL;
    root = build(value, N);
    int cmd, info1, info2, info3;
    while(Q--){
        scanf("%d", &cmd);
        if(cmd == 6) removeLargest(&root);
        else if(cmd == 1){
            scanf("%d%d", &info1, &info2);
            Node *temp = genTreapNode(info1, info2);
            insert(&root, temp, info2);
        }
        else if(cmd == 2){
            scanf("%d", &info1);
            delete(&root, info1, 0);
        }
        else if(cmd == 3){
            scanf("%d%d%d", &info1, &info2, &info3);
            addition(&root, info1, info2, info3);
        }
        else if(cmd == 4){
            scanf("%d%d", &info1, &info2);
            printf("%d\n", query(&root, info1, info2));
        }
        else{
            scanf("%d%d", &info1, &info2);
            reverse(&root, info1, info2);
        }
    }
}