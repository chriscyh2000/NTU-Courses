#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
typedef struct fib_heap_node Node;
typedef struct package Package;

int line_of_height[100005];
int real_line_of[100005];

typedef struct package{
    int height;
    struct package *prev;
    struct package *next;
}Package;

typedef struct productionLine{
    Package *front;
    Package *end;
}pLine;
typedef struct fib_heap_node{
    bool mark;
    int height;
    int degree;
    struct fib_heap_node *parent;
    struct fib_heap_node *child;
    struct fib_heap_node *left;
    struct fib_heap_node *right;
}Node;
typedef struct fib_heap{
    Node *max;
}fib_Heap;

fib_Heap H[100005]; //the line L has the heap H[L]
pLine production_line[100005]; //the line L has the package list production_line[L]
Node node[100005]; //the node node[h] has the package with height h
Package pack[100005]; // pack[h] is the package with the height h

void fib_insert(int size[], int height, int lineIndex){
    node[height].height = height;
    node[height].degree = 0;
    node[height].mark = false;
    node[height].child = NULL;
    node[height].parent = NULL;
    if(size[lineIndex] == 0){ //initialize the max of every heap before the procedure starts.
        H[lineIndex].max = &node[height];
        node[height].left = &node[height];
        node[height].right = &node[height];
        return;
    }
    H[lineIndex].max->right->left = &node[height];
    node[height].right = H[lineIndex].max->right;
    H[lineIndex].max->right = &node[height];
    node[height].left = H[lineIndex].max;
    if(height > H[lineIndex].max->height) H[lineIndex].max = &node[height];
    return;
}
void push(int size[], int height, int lineIndex){
    pack[height].height = height;
    if(size[lineIndex] == 0){
        pack[height].next = NULL;
        pack[height].prev = NULL;
        production_line[lineIndex].front = &pack[height];
        production_line[lineIndex].end = &pack[height];
        line_of_height[height] = lineIndex;
    }
    else{
        production_line[lineIndex].end->next = &pack[height];
        pack[height].prev = production_line[lineIndex].end;
        pack[height].next = NULL;
        production_line[lineIndex].end = &pack[height];
        line_of_height[height] = line_of_height[production_line[lineIndex].front->height];
    }
    fib_insert(size, height, lineIndex);
    ++size[lineIndex];
    return;
}
void consolidate(int size, int lineIndex){
    if(size == 0) return;
    int temp = (log(size)) / (log(2)) + 5;
    bool stop = false;
    Node *ptr[temp];
    for(int i = 0; i < temp; ++i)
        ptr[i] = NULL;
    Node *cur = H[lineIndex].max;
    while(ptr[cur->degree] != cur && !stop){
        if(ptr[cur->degree] == NULL){
            ptr[cur->degree] =cur;
            cur = cur->right;
        }
        else{
            Node *next = cur->right;
            Node *master, *slave;
            do{
                if(ptr[cur->degree]->height > cur->height){
                    master = ptr[cur->degree];
                    slave = cur;
                }
                else{
                    master = cur;
                    slave = ptr[cur->degree];
                }
                if(slave == next) stop = true;
                slave->left->right = slave->right;
                slave->right->left = slave->left;
                if(master->child == NULL){
                    master->child = slave;
                    slave->parent = master;
                    slave->right = slave;
                    slave->left = slave;
                }
                else if(master->child != NULL){
                    slave->parent = master;
                    master->child->right->left = slave;
                    slave->right = master->child->right;
                    master->child->right = slave;
                    slave->left = master->child;
                }
                ptr[master->degree] = NULL;
                ++master->degree;
                cur = master;
                if(ptr[cur->degree] == NULL)
                    ptr[cur->degree] = cur;
            }while(ptr[cur->degree] != cur);
            if(!stop) cur = next;
        }
    }
    H[lineIndex].max = cur;
    Node *stopNode = cur;
    cur = cur->right;
    while(cur != stopNode){
        if(cur->height > H[lineIndex].max->height)
            H[lineIndex].max = cur;
        cur = cur->right;
    }
    return;
}
void fib_extract_max(int size, int lineIndex){
    if(size == 0) return;
    Node *toExtract = H[lineIndex].max;
    if(toExtract->child == NULL && toExtract->right == toExtract){
        H[lineIndex].max = NULL;
        return;
    }
    if(toExtract->child != NULL){
        toExtract->child->parent = NULL;
        Node *cur = toExtract->child->right;
        while(cur != toExtract->child){
            cur->parent = NULL;
            cur = cur->right;
        }
        if(toExtract->right != toExtract){
            toExtract->right->left = toExtract->child->left;
            toExtract->child->left->right = toExtract->right;
            toExtract->left->right = toExtract->child;
            toExtract->child->left = toExtract->left;
            H[lineIndex].max = toExtract->right;
        }
        else H[lineIndex].max = cur;
    }
    else if(toExtract->child == NULL && toExtract->right != toExtract){
        toExtract->right->left = toExtract->left;
        toExtract->left->right = toExtract->right;
        H[lineIndex].max = toExtract->right;
    }
    consolidate(size, lineIndex);
    /* remember to decrease the size of production_line[lineIndex] 
        in pop procedure after calling fib_extract_max */
    return;
}
void pop_highest(int size[], int lineIndex){
    int height = H[lineIndex].max->height;
    if(pack[height].prev != NULL) pack[height].prev->next = pack[height].next;
    if(pack[height].next != NULL) pack[height].next->prev = pack[height].prev;
    if(&pack[height] == production_line[lineIndex].end)
        production_line[lineIndex].end = pack[height].prev;
    if(&pack[height] == production_line[lineIndex].front)
         production_line[lineIndex].front = pack[height].next;
    fib_extract_max(size[lineIndex], lineIndex);
    --size[lineIndex];
    return;
}
void fib_union(int size1, int size2, int lineIndex1, int lineIndex2){ // idx1 merged to idx2
    //remember to modify the size in merge function after calling fib_union
    if(size1 == 0) return;
    if(size2 == 0){
        H[lineIndex2].max = H[lineIndex1].max;
        H[lineIndex1].max = NULL;
        return;
    }
    Node *temp1 = H[lineIndex1].max->left;
    Node *temp2 = H[lineIndex2].max->right;
    H[lineIndex2].max->right = H[lineIndex1].max;
    H[lineIndex1].max->left = H[lineIndex2].max;
    temp2->left = temp1;
    temp1->right = temp2;
    if(H[lineIndex1].max->height > H[lineIndex2].max->height)
        H[lineIndex2].max = H[lineIndex1].max;
    H[lineIndex1].max = NULL;
    return;
}
void merge(int size[], int lineIndex1, int lineIndex2){
    if(size[lineIndex1] == 0) return;
    else if(size[lineIndex2] == 0){
        production_line[lineIndex2].front = production_line[lineIndex1].front;
        production_line[lineIndex2].end = production_line[lineIndex1].end;
        fib_union(size[lineIndex1], size[lineIndex2],lineIndex1, lineIndex2);
        size[lineIndex2] = size[lineIndex1];
        size[lineIndex1] = 0;
        int temp = line_of_height[production_line[lineIndex1].front->height];
        real_line_of[temp] = lineIndex2;
        return;
    }
    production_line[lineIndex2].end->next = production_line[lineIndex1].front;
    production_line[lineIndex1].front->prev = production_line[lineIndex2].end;
    Package *temp1 = production_line[lineIndex1].front;
    production_line[lineIndex2].end = production_line[lineIndex1].end;
    production_line[lineIndex1].front = NULL;
    production_line[lineIndex1].end = NULL;
    fib_union(size[lineIndex1], size[lineIndex2], lineIndex1, lineIndex2);
    if(size[lineIndex1] > size[lineIndex2]){
        Package *cur = production_line[lineIndex2].front;
        int temp = line_of_height[temp1->height];
        while(cur != temp1){
            line_of_height[cur->height] = temp;
            cur = cur->next;
        }
        real_line_of[temp] = lineIndex2;
    }
    else{
        Package *cur = temp1;
        int temp = line_of_height[production_line[lineIndex2].front->height];
        while(cur != NULL){
            line_of_height[cur->height] = temp;
            cur = cur->next;
        }
    }
    // remember to initialize the real_line of every line to itself
    // remember to initialize the line_of_height of every package to -1
    size[lineIndex2] += size[lineIndex1];
    size[lineIndex1] = 0;
    return;
}
void fib_cut(int lineIndex, Node *toCut, Node *parent){
    if(toCut->right == toCut)
        parent->child = NULL;
    (toCut->left)->right = toCut->right;
    (toCut->right)->left = toCut->left;
    if(parent->child == toCut)
        parent->child = toCut->right;
    --parent->degree;
    toCut->parent = NULL;
    toCut->left = H[lineIndex].max;
    toCut->right = H[lineIndex].max->right;
    H[lineIndex].max->right->left = toCut;
    H[lineIndex].max->right = toCut;
    toCut->mark = false;
    return;
}
void fib_cascading_cut(int lineIndex, Node *toCut, Node *parent){
    if(parent != NULL){
        if(!toCut->mark)
            toCut->mark = true;
        else{
            fib_cut(lineIndex, toCut, parent);
            fib_cascading_cut(lineIndex, parent, parent->parent);
        }
    }
    return;
}
void fib_increase_height(int originalHeight, int newHeight, int lineIndex){
    node[originalHeight].height = newHeight;
    Node *parent = node[originalHeight].parent;
    if(parent != NULL && parent->height < newHeight){
        fib_cut(lineIndex, &node[originalHeight], parent);
        fib_cascading_cut(lineIndex, parent, parent->parent);
    }
    if(node[originalHeight].height > H[lineIndex].max->height)
        H[lineIndex].max = &node[originalHeight];
    return;
}
void fib_delete(int size, int lineIndex, int height){
    fib_increase_height(height, 200000, lineIndex);
    fib_extract_max(size, lineIndex);
    return;
}
void pop_front(int size[], int lineIndex){
    if(size[lineIndex] == 0) return;
    Package *temp = production_line[lineIndex].front->next;
    int height = production_line[lineIndex].front->height;
    if(temp != NULL) temp->prev = NULL;
    production_line[lineIndex].front->next = NULL;
    production_line[lineIndex].front = temp;
    fib_delete(size[lineIndex], lineIndex, height);
    --size[lineIndex];
    if(size[lineIndex] == 0)
        production_line[lineIndex].end = NULL;
    return;
}
void pop_end(int size[], int lineIndex){
    if(size[lineIndex] == 0) return;
    Package *temp = production_line[lineIndex].end->prev;
    int height = production_line[lineIndex].end->height;
    if(temp != NULL) temp->next = NULL;
    production_line[lineIndex].end->prev = NULL;
    production_line[lineIndex].end = temp;
    fib_delete(size[lineIndex], lineIndex, height);
    --size[lineIndex];
    if(size[lineIndex] == 0)
        production_line[lineIndex].front = NULL;
    return;
}
int main(void){
    int T, N, O, L;
    scanf("%d", &T);
    for(int i = 0; i < T; ++i){
        scanf("%d %d %d", &N, &O, &L);
        int size[100000] = {0};
        for(int j = 0; j < L; ++j)
            real_line_of[j] = j;
        for(int j = 1; j <= N; ++j)
            line_of_height[j] = -1;
        char operation[O][6];
        int operation_information[O][2];
        int targetLine[N];
        for(int j = 0; j < O; ++j){
            scanf("%s", operation[j]);
            scanf("%d %d", &operation_information[j][0], &operation_information[j][1]);
        }
        for(int j = 0; j < N; ++j)
            scanf("%d", &targetLine[j]);
        int checkTarget = 0, executedOP = 0;
        
        
        while(checkTarget < N && executedOP < O){
            int line_num = line_of_height[targetLine[checkTarget]];
            int real_line_num;
            
            if(line_num != -1) real_line_num = real_line_of[line_num];
            while(((line_num == -1) || \
              (line_num != -1 && (H[real_line_num].max->height != targetLine[checkTarget] && \
               production_line[real_line_num].front->height != targetLine[checkTarget] && \
               production_line[real_line_num].end->height != targetLine[checkTarget]))) && \
              executedOP < O){
            
                if(operation[executedOP][0] == 'p'){
                    int height_toPush = operation_information[executedOP][0];
                    int lineIndex = operation_information[executedOP][1];
                    push(size, height_toPush, lineIndex);
                }
                else if(operation[executedOP][0] == 'm'){
                    int lineIndex1 = operation_information[executedOP][0];
                    int lineIndex2 = operation_information[executedOP][1];
                    merge(size, lineIndex1, lineIndex2);
                }
                ++executedOP;
                line_num = line_of_height[targetLine[checkTarget]];
                if(line_num != -1) real_line_num = real_line_of[line_num];
            }
            if(line_num != -1){
                if(H[real_line_num].max->height == targetLine[checkTarget]){
                    pop_highest(size, real_line_num);
                    ++checkTarget;
                }
                else if(production_line[real_line_num].front->height == targetLine[checkTarget]){
                    pop_front(size, real_line_num);
                    ++checkTarget;
                }
                else if(production_line[real_line_num].end->height == targetLine[checkTarget]){
                    pop_end(size, real_line_num);
                    ++checkTarget;
                }
            }
        }
        while(checkTarget < N && line_of_height[targetLine[checkTarget]] != -1){
            int line_num = line_of_height[targetLine[checkTarget]];
            int real_line_num = real_line_of[line_num];

            if(H[real_line_num].max->height == targetLine[checkTarget]){
                pop_highest(size, real_line_num);
                ++checkTarget;
            }
            else if(production_line[real_line_num].front->height == targetLine[checkTarget]){
                pop_front(size, real_line_num);
                ++checkTarget;
            }
            else if(production_line[real_line_num].end->height == targetLine[checkTarget]){
                pop_end(size, real_line_num);
                ++checkTarget;
            }
            else break;
        }
        if(checkTarget == N) printf("possible\n");
        else printf("impossible\n");
    }
}