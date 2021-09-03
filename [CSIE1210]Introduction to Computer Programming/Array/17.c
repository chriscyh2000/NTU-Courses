#include <stdio.h>
int main()
{
    int r, c;
    scanf("%d%d", &r, &c);
    int num[r][c], sum[100] = {0}, average[c];
    int i,j;
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            scanf("%d", &num[i][j]);
            sum[j] += num[i][j];
        }
    }
    for (i = 0; i < c; i++)
    {
        average[i] = sum[i] / r;
        printf("%d\n", average[i]);
    }
}