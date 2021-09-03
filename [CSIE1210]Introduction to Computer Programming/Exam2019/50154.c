#include <stdio.h>
int main()
{
    int a, b, c, d, e, f, g;
    scanf("%d%d%d%d%d%d%d", &a, &b, &c, &d, &e, &f, &g);
    int totaledge, totalpath;
    totaledge = a + b + c + d + a * e + b * e + c * e + c * f + d * f + e * g + f + g;
    totalpath = a * e * g + b * e * g + c * e * g + c * f + d * f;
    printf("%d\n%d\n", totaledge, totalpath);
}