#include "evaluate_f.h"
int evaluate_f(int *iptr[], int n, int *index)
{
    int i;
    int max = (*iptr[0]) * 4 - (*(iptr[0] + 1)) * 6;
    for (i = 0; i < n; i++)
    {
        if (max < (*iptr[i]) * 4 - (*(iptr[i] + 1)) * 6)
        {
            max = (*iptr[i]) * 4 - (*(iptr[i] + 1)) * 6;
            *index = i;
        }
    }
    return max;
}