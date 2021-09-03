#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    int num[102][102]={0};//0代表建築物，1代表路。
    int typesum[4] = {0};//type0:十字路口，type1:T字路口，type2:彎道，type3:死路。
    int i, j;
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            scanf("%d", &num[i][j]);
        }
    }
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if (num[i][j] == 1)
            {
                if (num[i-1][j] + num[i+1][j] + num[i][j-1] + num[i][j+1] == 4)
                {
                    typesum[0]++;
                }
                else if (num[i-1][j] + num[i+1][j] + num[i][j-1] + num[i][j+1] == 3)
                {
                    typesum[1]++;
                }
                else if ((num[i-1][j] + num[i+1][j] + num[i][j-1] + num[i][j+1] == 2) && (num[i-1][j] != num[i+1][j]))//左右兩邊是建築,上下是路,這種不是彎道,所以要排除
                {
                    typesum[2]++;
                }
                else if (num[i-1][j] + num[i+1][j] + num[i][j-1] + num[i][j+1] == 1)
                {
                    typesum[3]++;
                } 
            }
        }
    }
    for (i = 0; i < 4; i++)
    {
        printf("%d\n", typesum[i]);
    }
}