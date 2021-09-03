#include "node.h"
#include <stdio.h>
#include <stdlib.h>

struct node *getNode(struct node *head, unsigned int i)
{
    struct node *current = head;
    unsigned int n = 0;
    while(current->next != NULL)
    {
        current->next->prev = current;
        current = current->next;
        n++;
    }
    if(i > n) return NULL;
    for(unsigned int j = 0; j < i; j++)
        current = current->prev;
    return current;
}