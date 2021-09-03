#include <stdio.h>
int main(){
    int a, b, c, x;
    scanf("%d%d%d", &a, &b, &c);
    x = (a + b > c) && (a + c > b) && (b + c > a);
    printf("%d\n", x);
}