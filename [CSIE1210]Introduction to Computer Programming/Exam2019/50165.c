#include <stdio.h>
#include <math.h>
int main(void){
    int num;
    double a1, b1, c1, a2, b2, c2, a3, b3, c3;
    double x1, y1, x2, y2, x3, y3;
    double total = 0;
    scanf("%d", &num);
    scanf("%lf%lf%lf", &a1, &b1, &c1);
    scanf("%lf%lf%lf", &a2, &b2, &c2);
    x1 = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
    y1 = (a1 * c2 - c1 * a2) / (a2 * b1 - a1 * b2);
    a3 = a1;
    b3 = b1;
    c3 = c1;
    x3 = x1;
    y3 = y1;
    for(int i = 0; i < num - 2; ++i){
        a1 = a2;
        b1 = b2;
        c1 = c2;
        scanf("%lf%lf%lf", &a2, &b2, &c2);
        x2 = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
        y2 = (a1 * c2 - c1 * a2) / (a2 * b1 - a1 * b2);
        total += sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        x1 = x2;
        y1 = y2;
    }
    a1 = a3;
    b1 = b3;
    c1 = c3;
    x2 = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
    y2 = (a1 * c2 - c1 * a2) / (a2 * b1 - a1 * b2);
    total += sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    total += sqrt((x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3));
    printf("%0.3lf\n", total);
}