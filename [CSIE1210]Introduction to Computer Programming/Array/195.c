#include <stdio.h>
int main()
{
    int N;
    scanf("%d", &N);
    int x[N], y[N];
    int position[3][3] = {0};//1代表放黑棋 2代表放白棋 0代表沒東西
    int blackstep = 0, whitestep = 0;
    int winner = 0;
    int i;
    int a, b;
    for (i = 0; i < N; i++)
    {
        scanf("%d%d", &x[i], &y[i]);
        a = x[i]; b = y[i];
        if (winner != 0)
        {
            continue;
        }
        if ((a > 2) || (a < 0) || (b >2) || (b < 0))
        {
            continue;
        }
        if (position[a][b] == 0)
        {
            if (blackstep == whitestep)
            {
                position[a][b] = 1;
                blackstep++;
            }
            else if (blackstep > whitestep)
            {
                position[a][b] = 2;
                whitestep++;
            }
        }
        else 
        {
            continue;
        }
        if(((position[0][0] == 1) && (position[0][1] == 1) && (position[0][2] == 1)) ||\
        ((position[1][0] == 1) && (position[1][1] == 1) && (position[1][2] == 1)) ||\
        ((position[2][0] == 1) && (position[2][1] == 1) && (position[2][2] == 1)) ||\
        ((position[0][0] == 1) && (position[1][0] == 1) && (position[2][0] == 1)) ||\
        ((position[0][1] == 1) && (position[1][1] == 1) && (position[2][1] == 1)) ||\
        ((position[0][2] == 1) && (position[1][2] == 1) && (position[2][2] == 1)) ||\
        ((position[0][0] == 1) && (position[1][1] == 1) && (position[2][2] == 1)) ||\
        ((position[0][2] == 1) && (position[1][1] == 1) && (position[2][0] == 1)))//判斷是否有連線有的話就紀錄沒的話就繼續
        {
            winner = 1;
        }
        else if(((position[0][0] == 2) && (position[0][1] == 2) && (position[0][2] == 2)) ||\
        ((position[1][0] == 2) && (position[1][1] == 2) && (position[1][2] == 2)) ||\
        ((position[2][0] == 2) && (position[2][1] == 2) && (position[2][2] == 2)) ||\
        ((position[0][0] == 2) && (position[1][0] == 2) && (position[2][0] == 2)) ||\
        ((position[0][1] == 2) && (position[1][1] == 2) && (position[2][1] == 2)) ||\
        ((position[0][2] == 2) && (position[1][2] == 2) && (position[2][2] == 2)) ||\
        ((position[0][0] == 2) && (position[1][1] == 2) && (position[2][2] == 2)) ||\
        ((position[0][2] == 2) && (position[1][1] == 2) && (position[2][0] == 2)))
        {
            winner = 2;
        }
    }
    if (winner == 1)
    {
        printf("Black wins.\n");
    }
    else if (winner == 2)
    {
        printf("White wins.\n");
    }
    else
    {
        printf("There is a draw.\n");
    }
}