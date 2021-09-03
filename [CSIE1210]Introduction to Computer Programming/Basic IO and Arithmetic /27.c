#include <stdio.h>
int main()
{
    int a, b, c, d, e, v, s, vd1, vd2, vd3;
    scanf("%d%d%d%d%d", &a, &b, &c, &d, &e);
   
    vd1 = d * (c - 2 * e) * (a - 2 * e) * 2;
    vd2 = d * (b - 2 * e) * (a - 2 * e) * 2;
    vd3 = d * (b - 2 * e) * (c - 2 * e) * 2;
    v = a * b * c - vd1 - vd2 - vd3;
    s = (a * b + a * c + b * c) * 2\
    + (a - 2 * e) * d * 8 + (b - 2 * e) * d * 8 + (c - 2 * e) * d * 8;
    printf("%d\n%d\n", s, v);
}