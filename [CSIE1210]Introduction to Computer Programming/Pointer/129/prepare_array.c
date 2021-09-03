#include "prepare_array.h"
void prepare_array(int buffer[], int *array[], int row, int column[])
{
    int i;
    int start = 0; 
    for (i = 0; i < row; i++)
    {
        array[i] = &(buffer[start]);
        start += column[i]; 
    }
    return;
}