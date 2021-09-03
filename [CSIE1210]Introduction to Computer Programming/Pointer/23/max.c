#include "max.h"
int max(int *iptr[], int n)
{
    int i, j;
    for (i = 0; i <= n - 2; i++)
    {
        if(*iptr[i] > *iptr[i+1])
        {
            int temp;
            temp = *iptr[i];
            *iptr[i] = *iptr[i+1];
            *iptr[i+1] = temp;
        }
    }
    return *iptr[n-1];
}