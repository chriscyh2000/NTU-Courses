#include "snake.h"
void snake(const int *ptr_array[100][100], int m)
{
    int *array[10000];
    int i, j;
    for (i = 0; i < m; i++)
    {
      for(j = 0; j < m; j++)
      {
        array[m*i+j] = ptr_array[i][j];
      }
    }
    for(i = m * m - 2; i >= 0; i--)
    {
      for (j = 0; j <= i; j++)
      {
        if(*array[j] > *array[j+1])
        {
          int *temp;
          temp = array[j];
          array[j] = array[j+1];
          array[j+1] = temp; 
        }
      }
    }
    for(i = 0;i < m; i++)
    {
        for(j = 0;j < m; j++)
        {
            if(i % 2 == 0)
            {
                ptr_array[i][j] = array[i*m+j];
            }
            else
            {
                ptr_array[i][m-1-j] = array[i*m+j];
            }
        }
    }
}