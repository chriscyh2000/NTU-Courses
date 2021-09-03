#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void initMemory(Memory *memory, int length)
{
    memory->start = 0;
    memory->length = length;
    memory->next = NULL;
    return;
}
void printMemory(Memory *memory)
{
    Memory *current = memory;
    printf("==========\n");
    if(memory->start == -1 && memory->length == -1) return;
    while(current != NULL)
    {
        printf("start %d, length %d\n", current->start, current->length);
        current = current->next;
    }
    return;
}
Memory *genBlockNode(int start, int length, Memory *next)
{
    Memory *temp = (Memory *)malloc(sizeof(Memory));
    assert(temp != NULL);
    temp->start = start;
    temp->length = length;
    temp->next = next;
    return temp;
}
void allocateMemory(Memory *memory, int start, int length)
{
    Memory *current = memory, *prev = NULL;
    while((current != NULL) && (start >= current->start + current->length + 1))
    {
        prev = current;
        current = current->next;
    }
    assert((current != NULL) && 
          ((current->start <= start) && (start + length <= current->start + current->length)));
    bool adjacentStart = (start == current->start);
    bool adjacentEnd = (start + length == current->start + current->length);
    if(adjacentStart && adjacentEnd)
    {
        if(prev == NULL)
        {
            if(memory->next != NULL)
            {
                memory->start = memory->next->start;
                memory->length = memory->next->length;
                Memory *temp = memory->next->next;
                free(memory->next);
                memory->next = temp;
            }
            else
            {
                memory->start = -1;
                memory->length = -1;
            }
        }
        else
        {
            prev->next = current->next;
            free(current);
        }
    }
    else if(adjacentStart)
    {
        current->length -= length;
        current->start += length;
    }
    else if(adjacentEnd)
        current->length -= length;
    else
    {
        Memory *temp = genBlockNode(start + length,\
                     (current->start + current->length) - (start + length), current->next);
        current->length = start - current->start;
        current->next = temp;
    }
    return;
}
void freeMemory(Memory *memory, int start, int length)
{
    Memory *current = memory, *prev = NULL;
    if(memory->start == -1 && memory->length == -1)
    {
        memory->start = start;
        memory->length = length;
        return;
    }
    while(current != NULL && current->start < start)
    {
        prev = current;
        current = current->next;
    }
    bool adjacentPrev = ((prev != NULL) && (prev->start + prev->length == start));
    assert((prev == NULL) ||\
          ((prev != NULL) && (start >= prev->start + prev->length)));
    bool adjacentCurrent = ((current != NULL) && (current->start == start + length));
    assert((current == NULL) ||\
          ((current != NULL) && (current->start >= start + length)));
    if(adjacentCurrent && adjacentPrev)
    {
        prev->length = prev->length + length + current->length;
        prev->next = current->next;
        free(current);
    }
    else if(adjacentCurrent)
    {
        current->start = start;
        current->length += length;
    }
    else if(adjacentPrev)
        prev->length += length;
    else
    {
        if(prev == NULL)
        {
            memory->next = genBlockNode(current->start, current->length, current->next);
            memory->start = start;
            memory->length = length;
        }
        else 
            prev->next = genBlockNode(start, length, current);
    }
    return;
}
