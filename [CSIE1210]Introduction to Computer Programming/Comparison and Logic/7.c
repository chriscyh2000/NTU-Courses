#include <stdio.h>
int main(){
    int x, y;
    scanf("%d", &x);
    y = (x % 3 != 0) || (x % 5 != 0) || (x % 7 == 0);
    printf("%d\n", y);
}