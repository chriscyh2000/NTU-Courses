#include <stdio.h>
int main(){
    int M, N, x1i, y1i, e1, n1, f1, x2i, y2i, e2, n2, f2; //x y 可大於等於0但不可等於Ｍ Ｎ
    scanf("%d%d", &M, &N);
    scanf("%d%d%d%d%d", &x1i, &y1i, &e1, &n1, &f1);
    scanf("%d%d%d%d%d", &x2i, &y2i, &e2, &n2, &f2);
    int processtime;
    if (f1 > f2)
    {
        processtime = f1;
    }
    else
    {
        processtime = f2;
    }
    int x1[processtime+3], y1[processtime+3], x2[processtime+3], y2[processtime+3];
    x1[0] = x1i; y1[0] = y1i; x2[0] = x2i; y2[0] = y2i;
    int nstep1 = 0, estep1 = 0, nstep2=0, estep2=0;
    int i;
    int explodetime;
    for (i = 0; i <= processtime + 1; i++)
    {
        if (f1 != 0)
        {
            if(nstep1 != n1)
            {
                y1[i+1] = y1[i] + 1; x1[i+1] = x1[i];
                if (y1[i+1] == N)
                {
                    y1[i+1] = 0;
                }
                nstep1++;
            }
            else if (estep1 != e1)
            {
                x1[i+1] = x1[i] + 1; y1[i+1] = y1[i];
                if (x1[i+1] == M)
                {
                    x1[i+1] = 0;
                }
                estep1++;
            }
            else
            {
                nstep1 = estep1 = 0;
                y1[i+1] = y1[i] +1; x1[i+1] = x1[i];
                nstep1++;
                if (y1[i+1] == N)
                {
                    y1[i+1] = 0;
                } 
            }
            f1--;
        }
        else
        {
            x1[i+1] = x1[i];
            y1[i+1] = y1[i];
        }
        if (f2 != 0)
        {
            if(estep2 != e2)
            {
                x2[i+1] = x2[i] + 1; y2[i+1] = y2[i];
                if (x2[i+1] == M)
                {
                    x2[i+1] = 0;
                }
                estep2++;
            }
            else if (nstep2 != n2)
            {
                y2[i+1] = y2[i] + 1; x2[i+1] = x2[i];
                if (y2[i+1] == N)
                {
                    y2[i+1] = 0;
                }
                nstep2++;
            }
            else
            {
                nstep2 = estep2 = 0;
                x2[i+1] = x2[i] +1; y2[i+1] = y2[i];
                estep2++;
                if (x2[i+1] == N)
                {
                    x2[i+1] = 0;
                } 
            }
            f2--;
        }
        else
        {
            x2[i+1] = x2[i];
            y2[i+1] = y2[i];
        }
        if ((x1[i+1] == x2[i+1])&&(y1[i+1] == y2[i+1]))
        {
            explodetime = i + 1;
            break;
        }
    }
    if (i < processtime + 1)
    {
        printf("robots explode at time %d\n", explodetime);
    }
    else
    {
        printf("robots will not explode\n");
    }
}