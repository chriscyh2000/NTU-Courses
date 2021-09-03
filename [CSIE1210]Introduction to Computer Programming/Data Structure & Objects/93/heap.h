#ifndef HEAP_H
#define HEAP_H
#define MAXHEAP 100
 
struct Heap{
    int ary[MAXHEAP];
    int num;
};
 
void initialize(struct Heap *heap);
int removeMin(struct Heap *heap);
void add(struct Heap *heap, int i);
int isFull(struct Heap *heap);
int isEmpty(struct Heap *heap);
#endif