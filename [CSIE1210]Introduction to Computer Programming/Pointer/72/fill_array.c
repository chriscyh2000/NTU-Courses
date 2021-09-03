#include "fill_array.h"
void fill_array(int *ptr[], int n)
{
    int i, j;
    int *order[n];
    order[0] = ptr[0];
    for (int *k = ptr[0]; k <= ptr[n-1]; k++)
        *k = 0;
    for (i = 0; i < n; i++)
    {
        *ptr[i] = i;
    }
    for (i = 0; i < n - 1; i++)
    {
        j = 1;
        while (*(order[i] + j) <= 0)
        {
            j++;
        }
        order[i+1] = order[i] + j;
        j = 1;
        while(order[i] + j != order[i+1])
        {
            *(order[i] + j) = *order[i] + *order[i+1];
            j++;
        }
    }
    return;
}