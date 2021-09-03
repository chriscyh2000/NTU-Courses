#include <stdio.h>
int main(){
    int n1, n2;
    int time = 0;
    scanf("%d%d", &n1, &n2);
    if(n1 == (n2 / 100000)) time++;
    if(n1 == ((n2 / 10000) % 100)) time++;
    if(n1 == ((n2 / 1000) % 100)) time++;
    if(n1 == ((n2 / 100) % 100)) time++;
    if(n1 == ((n2 / 10) % 100)) time++;
    if(n1 == (n2 % 100)) time++;
    printf("%d\n", time);
}