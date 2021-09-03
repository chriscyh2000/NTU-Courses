#include <stdio.h>
int main()
{
    int a, b, c, d, e, f;
    int perimeter, area;
    scanf("%d%d%d%d%d%d", &a, &b, &c, &d, &e, &f);
    perimeter = (a + c + e) * 2 + (b + d + f) * 2;
    area = a * b + (a + c) * d + (a + c + e) * f;
    printf("%d\n%d\n", perimeter, area);
}