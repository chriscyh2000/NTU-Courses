#include <stdio.h>
#include "card.h"
void shuffle(int *deck[])
{
    int i;
    int cardnum;
    for (i = 0; i < 10000; i++)
    {
        if (deck[i] == NULL)
        {
        cardnum = i;
        break;
        }
    }
    int start2;
    start2 = (cardnum % 2 == 0)? (cardnum / 2) : (cardnum / 2 + 1);
    int *aftershuffle[cardnum];
    for (i = 0; i < start2; i ++)
    {
        aftershuffle[i * 2] = deck[i];
    }
    for (i = 0; start2 + i < cardnum; i ++)
    {
        aftershuffle[i * 2 + 1] = deck[start2 + i];
    }
    for(i = 0; i < cardnum; i++)
        deck[i] = aftershuffle[i];
    return;
}
void print(int *deck[])
{
    if (deck[0] != NULL)
    {
        int i = 1;
        printf("%d", *deck[0]);
        while(deck[i] != NULL)
        {
            printf(" %d", *deck[i]);
            i++;
        }
        printf("\n");
    }
    return;
}