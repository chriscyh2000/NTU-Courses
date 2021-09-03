#include "heap.h"
#include <stdio.h>
int main(void)
{
    struct Heap heap;
    int addNum;
    initialize(&heap);
    while(scanf("%d", &addNum) != EOF)
    {
        add(&heap, addNum);
    }
    while(isEmpty(&heap) == 0)
        printf("%d\n", removeMin(&heap));
    return 0;
}