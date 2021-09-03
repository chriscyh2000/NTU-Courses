#include "posneg.h"
void posneg(int array[5][5], int result[2])
{
    result[0] = result[1] = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (array[i][j] > 0)
                result[1]++;
            else if (array[i][j] < 0)
                result[0]++;
        }
    }
}