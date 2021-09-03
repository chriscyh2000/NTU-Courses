#include "node.h"
#include <stdio.h>
struct node *merge(struct node *list1, struct node *list2)
{
    struct node *head, *current;
    struct node *candidate1 = NULL, *candidate2 = NULL;
    if(list1->value < list2->value) 
    {
        head = list1;
        candidate1 = head->next;
        candidate2 = list2;
    }
    else
    {
        head = list2;
        candidate1 = head->next;
        candidate2 = list1;
    }
    current = head;
    while(candidate1 != NULL || candidate2 != NULL)
    {
        struct node *selectedNode;
        if((candidate1 == NULL && candidate2 != NULL) ||\
           (candidate1 != NULL && candidate2 != NULL && candidate1->value > candidate2->value))
        {
            current->next = candidate2;
            current = current->next;
            candidate2 = current->next;
        }
        if((candidate1 != NULL && candidate2 == NULL) ||\
           (candidate1 != NULL && candidate2 != NULL && candidate1->value < candidate2->value))
        {
            current->next = candidate1;
            current = current->next;
            candidate1 = current->next;
        }
    }
    return head;
}