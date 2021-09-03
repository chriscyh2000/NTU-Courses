#include <stdio.h>
#include <assert.h>
#define pi 3.1415926

int main(void)
{
    double a, b, c, area;
    scanf("%lf%lf%lf", &a, &b, &c);
    assert(a + b >= c);
    area = pi * c * c * (3.0 / 4.0);
    if (b < c)
    {
        if(a < c)
            area += pi * (c - b) * (c - b) / 4 + pi * (c - a) * (c - a) / 4;
        else 
            area += pi * (c - b) * (c - b) / 4;
    }
    else
    {
        if (a < c)
            area += pi * (c - a) * (c - a) / 4;
    }
    printf("%f\n", area);
    return 0;
}