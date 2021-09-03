#include <stdio.h>
int main()
{
    int r, c, a, b;
    scanf("%d%d", &r, &c);
    a = r + 2;  b = c + 2;//a&b為我們方便比較所改造成之陣列的row&column  
    int num[102][102] = {0};
    int i, j;
    for (i = 1; i < a - 1; i++)
    {
        for (j = 1; j < b -1; j++)
        {
            scanf("%d", &num[i][j]);
        }
    }
    for (i = 1; i < a - 1; i++)
    {
        for (j = 1; j<  b - 1; j++)
        {
            if ((num[i][j] > num[i-1][j]) && (num[i][j] > num[i+1][j]) && (num[i][j] > num[i][j-1]) && (num[i][j] > num[i][j+1]))
            {
                printf("%d\n", num[i][j]);
            }
        }
    }
}