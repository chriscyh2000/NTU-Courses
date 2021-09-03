#include <stdio.h>
int main(void)
{
    int x;
    int caltemp, tempforx2, tempforx1;
    int tempx1 = 0, tempx2 = 0, tempx3 = 0;
    int coftempx3 = 0, coftempx1 = 0, coftempx2 = 0;
    int tempforcx2, tempforcx1;
    int i = 0;
    int n = 0;
    int a, b, c, d, e;
    scanf("%d%d%d%d%d", &a, &b, &c, &d, &e);
    while(scanf("%d", &x) != EOF)
    {
        caltemp = x;
        tempforx2 = tempx3; tempforx1 = tempx2;
        tempx2 = tempforx2; tempx1 = tempforx1;
        tempx3 = x;
        tempforcx2 = coftempx3; tempforcx1 = coftempx2;
        coftempx2 = tempforcx2; coftempx1 = tempforcx1;
        coftempx3 = 0;
        while((caltemp < d) || (caltemp > e))
        {
            caltemp = (a * caltemp + b) % c;
            coftempx3++;
        }
        if (n == 1)
        {
            i++;
            continue;
        }
        if (i >= 2)
        {
            if (((coftempx3+1 == coftempx2) && (coftempx2+1 == coftempx1)) ||//
            ((coftempx3+1 == coftempx1) && (coftempx1+1 == coftempx2)) ||//
            ((coftempx2+1 == coftempx3) && (coftempx3+1 == coftempx1)) ||//
            ((coftempx2+1 == coftempx1) && (coftempx1+1 == coftempx3)) ||//
            ((coftempx1+1 == coftempx3) && (coftempx3+1 == coftempx2)) ||//
            ((coftempx1+1 == coftempx2) && (coftempx2+1 == coftempx3)))
            {
                printf("%d %d %d\n", tempx1, tempx2, tempx3);
                n = 1;
            }
        }
        i++;
    }
    if (n == 0)
        printf("Not found\n");
    return 0;
}