#include "heap.h"
void initialize(struct Heap *heap)
{
    heap->num = 0;
}
int isEmpty(struct Heap *heap)
{
    if(heap->num <= 0) return 1;
    return 0;
}
int isFull(struct Heap *heap)
{
    if(heap->num >= MAXHEAP) return 1;
    return 0;
}
void add(struct Heap *heap, int i)
{
    if(isFull(heap) == 0)
    {
        heap->ary[heap->num] = i;
        heap->num++;
        return;
    }
    return;
}
int removeMin(struct Heap *heap)
{
    if(isEmpty(heap) == 0)
    {
        int min = heap->ary[0], minIndex = 0;
        int i;
        for(i = 0; i < heap->num; i++)
        {
            if(heap->ary[i] < min)
            {
                min = heap->ary[i];
                minIndex = i;
            }
        }
        for(i = minIndex; i < heap->num - 1; i++)
        {
            heap->ary[i] = heap->ary[i+1];
            if(i == heap->num - 2) heap->ary[i+1] = 0;
        }
        heap->num--;
        return min;
    }
    return (-1);
}