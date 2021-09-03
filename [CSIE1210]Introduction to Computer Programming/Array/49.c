#include <stdio.h>
int remai[1000][10000] = {0};
int main()
{
    int n;
    scanf("%d", &n);
    int number[n];
    int m;
    scanf("%d", &m);
    int amount[1000] = {0};
    int i, j, k;
    for (i = 0; i < n; i++)
    {
        scanf("%d", &number[i]);
        for (j = 0; j < m; j++)
        {
            if(number[i] % m == j)
            {
                k = amount[j];
                remai[j][k] = number[i];
                amount[j] ++;
                break;
            }
        }
    }
    int sum[1000] = {0};
    int max[1000] = {0};
    int min[1000] = {0};
    for(i = 0; i < m; i++)
    {    
        max[i] = min[i] = remai[i][0];
        for (j = 0; j < amount[i]; j++)
        {
            sum[i] += remai[i][j];
            if (max[i] < remai[i][j])
            {
                max[i] = remai[i][j];
            }
            if (min[i] > remai[i][j])
            {
                min[i] = remai[i][j];
            }
        }
        printf("%d %d %d\n", sum[i], max[i], min[i]);
    }
}