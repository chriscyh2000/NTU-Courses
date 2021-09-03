#include <stdio.h>
int main(){
    int a, b, c, d, e, f, g, h, u, v, x, y, k;
    scanf("%d%d%d%d%d%d%d%d", &a, &b, &c, &d, &e, &f, &g, &h);
    u = a * d - c * b;
    v = c * f - e * d;
    x = e * h - g * f;
    y = g * b - a * h;
    k = x + y + u + v;
    k = (u > 0) && (v > 0) && (x > 0) && (y > 0);
    printf("%d\n", k);
}