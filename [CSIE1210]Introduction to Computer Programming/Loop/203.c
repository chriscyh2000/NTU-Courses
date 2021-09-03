#include <stdio.h>
int main(){
    int X, Y, t; 
    int x1i, y1i, x2i, y2i;
    int dx1i, dy1i, dx2i, dy2i;
    scanf("%d%d%d%d%d%d%d%d%d%d%d", &X, &Y, &x1i, &y1i, &x2i, &y2i, &dx1i, &dy1i, &dx2i, &dy2i, &t);
    int x1[t+1], y1[t+1], x2[t+1], y2[t+1];
    int dx1[t+1], dy1[t+1], dx2[t+1], dy2[t+1];
    x1[0] = x1i; y1[0] = y1i; x2[0] = x2i; y2[0] = y2i; 
    dx1[0] = dx1i; dy1[0] = dy1i; dx2[0] = dx2i; dy2[0] = dy2i;
    int i;
    for (i = 0; i <= t; i++)
    {
        if ((x1[i] + dx1[i] == X + 1) && (y1[i] + dy1[i] != Y + 1) && (y1[i] + dy1[i] != 0)) //撞右壁 (先討論1號球)
        {
            if (dy1[i] == 1)
            {
                dx1[i] = -1; dy1[i] = 1;
                dx1[i+1] = -1; dy1[i+1] = 1;
            }
            else if (dy1[i] == -1)
            {
                dx1[i] = -1; dy1[i] = -1;
                dx1[i+1] = -1; dy1[i+1] = -1;
            }
        }
        else if ((x1[i] + dx1[i] != 0) && (x1[i] + dx1[i] != X + 1) && (y1[i] + dy1[i] == Y + 1))//撞上壁
        {
            if (dx1[i] == 1)
            {
                dx1[i] = 1; dy1[i] = -1;
                dx1[i+1] = 1; dy1[i+1] = -1;
            }
            else if (dx1[i] == -1)
            {
                dx1[i] = -1; dy1[i] = -1;
                dx1[i+1] = -1; dy1[i+1] = -1; 
            }
        }
        else if ((x1[i] + dx1[i] == 0) && (y1[i] + dy1[i] != Y + 1) && (y1[i] + dy1[i] != 0)) //撞左壁
        {
            if (dy1[i] == 1)
            {
                dx1[i] = 1, dy1[i] = 1;
                dx1[i+1] = 1, dy1[i+1] = 1;
            }
            else if (dy1[i] == -1)
            {
                dx1[i] = 1, dy1[i] = -1;
                dx1[i+1] = 1, dy1[i+1] = -1;
            }
        }
        else if ((y1[i] + dy1[i] == 0) && (x1[i] + dx1[i] != X + 1) && (x1[i] + dx1[i] != 0)) //撞下壁
        {
            if (dx1[i] == 1)
            {
                dx1[i] = 1, dy1[i] = 1;
                dx1[i+1] = 1, dy1[i+1] = 1;
            }
            else if (dx1[i] == -1)
            {
                dx1[i] = -1, dy1[i] = 1;
                dx1[i+1] = -1, dy1[i+1] = 1;
            }    
        }
        else if (((x1[i] + dx1[i] == 0) && (y1[i] + dy1[i] == 0)) ||\
        ((x1[i] + dx1[i] == X + 1) && (y1[i] + dy1[i] == 0)) ||\
        ((x1[i] + dx1[i] == X + 1) && (y1[i] + dy1[i] == Y + 1)) ||\
        ((x1[i] + dx1[i] == 0) && (y1[i] + dy1[i] == Y + 1)))
        {
            dx1[i] = - dx1[i]; dy1[i] = - dy1[i];
            dx1[i+1] = dx1[i]; dy1[i+1] = dy1[i];
        }
        else if ((x1[i] + dx1[i] == x2[i] + dx2[i]) && (y1[i] + dy1[i] == y2[i] + dy2[i])) //相撞
        {
            dx1[i+1] = dx2[i]; dy1[i+1] = dy2[i];
        }
        else
        {
            dx1[i] = dx1[i]; dy1[i] = dy1[i];
            dx1[i+1] = dx1[i]; dy1[i+1] = dy1[i];
        }    
        x1[i+1] = x1[i] + dx1[i]; y1[i+1] = y1[i] +dy1[i];
        //間隔 以下做2號球
        if ((x2[i] + dx2[i] == X + 1) && (y2[i] + dy2[i] != Y + 1) && (y2[i] + dy2[i] != 0)) //撞右壁 (再討論2號球)
        {
            if (dy2[i] == 1)
            {
                dx2[i] = -1; dy2[i] = 1;
                dx2[i+1] = -1; dy2[i+1] = 1;
            }
            else if (dy2[i] == -1)
            {
                dx2[i] = -1; dy2[i] = -1;
                dx2[i+1] = -1; dy2[i+1] = -1;
            }
        }
        else if ((x2[i] + dx2[i] != 0) && (x2[i] + dx2[i] != X + 1) && (y2[i] + dy2[i] == Y + 1))//撞上壁
        {
            if (dx2[i] == 1)
            {
                dx2[i] = 1; dy2[i] = -1;
                dx2[i+1] = 1; dy2[i+1] = -1;
            }
            else if (dx2[i] == -1)
            {
                dx2[i] = -1; dy2[i] = -1;
                dx2[i+1] = -1; dy2[i+1] = -1; 
            }
        }
        else if ((x2[i] + dx2[i] == 0) && (y2[i] + dy2[i] != Y + 1) && (y2[i] + dy2[i] != 0)) //撞左壁
        {
            if (dy2[i] == 1)
            {
                dx2[i] = 1, dy2[i] = 1;
                dx2[i+1] = 1, dy2[i+1] = 1;
            }
            else if (dy2[i] == -1)
            {
                dx2[i] = 1, dy2[i] = -1;
                dx2[i+1] = 1, dy2[i+1] = -1;
            }
        }
        else if ((y2[i] + dy2[i] == 0) && (x2[i] + dx2[i] != X + 1) && (x2[i] + dx2[i] != 0)) //撞下壁
        {
            if (dx2[i] == 1)
            {
                dx2[i] = 1, dy2[i] = 1;
                dx2[i+1] = 1, dy2[i+1] = 1;
            }
            else if (dx2[i] == -1)
            {
                dx2[i] = -1, dy2[i] = 1;
                dx2[i+1] = -1, dy2[i+1] = 1;
            }    
        }
        else if (((x2[i] + dx2[i] == 0) && (y2[i] + dy2[i] == 0)) ||\
        ((x2[i] + dx2[i] == X + 1) && (y2[i] + dy2[i] == 0)) ||\
        ((x2[i] + dx2[i] == X + 1) && (y2[i] + dy2[i] == Y + 1)) ||\
        ((x2[i] + dx2[i] == 0) && (y2[i] + dy2[i] == Y + 1)))
        {
            dx2[i] = - dx2[i]; dy2[i] = - dy2[i];
            dx2[i+1] = dx2[i]; dy2[i+1] = dy2[i];
        }
        else if ((x2[i] + dx2[i] == x1[i] + dx1[i]) && (y2[i] + dy2[i] == y1[i] + dy1[i])) //相撞
        {
            dx2[i+1] = dx1[i]; dy2[i+1] = dy1[i];
        }
        else
        {
            dx2[i] = dx2[i]; dy2[i] = dy2[i];
            dx2[i+1] = dx2[i]; dy2[i+1] = dy2[i];
        }    
        x2[i+1] = x2[i] + dx2[i]; y2[i+1] = y2[i] + dy2[i];
        if ((x1[i] + dx1[i] == x2[i] + dx2[i]) && (y1[i] + dy1[i] == y2[i] + dy2[i])) //相撞
        {
            dx1[i+1] = dx2[i]; dy1[i+1] = dy2[i];
        }
        if ((x2[i] + dx2[i] == x1[i] + dx1[i]) && (y2[i] + dy2[i] == y1[i] + dy1[i])) //相撞
        {
            dx2[i+1] = dx1[i]; dy2[i+1] = dy1[i];
        }
    }
    printf("%d\n%d\n%d\n%d\n", x1[t], y1[t], x2[t], y2[t]);
}