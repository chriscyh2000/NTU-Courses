#include "intersection.h"
void intersection(int map[100][100], int result[4])
{
    result[0] = result[1] = result[2] = result[3] = 0;
    int testmap[102][102] = {{0}};
    for (int i = 1; i < 101; i++)
    {
        for (int j = 1; j < 101; j++)
        {
            testmap[i][j] = map[i-1][j-1];
        }
    }
    for (int i = 1; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            if (testmap[i][j] != 1)
                continue;
            int neighborsum = testmap[i][j-1] + testmap[i-1][j] + testmap[i][j+1] + testmap[i+1][j];
            if (neighborsum == 4)
                result[0]++;
            else if (neighborsum == 3)
                result[1]++;
            else if ((neighborsum == 2) && (testmap[i-1][j] != testmap[i+1][j]))
                result[2]++;
            else if (neighborsum == 1)
                result[3]++;
        }
    }
    return;
}