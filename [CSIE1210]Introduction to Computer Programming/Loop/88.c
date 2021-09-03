#include <stdio.h>
int main(){
    int a = 0, b = 0, n1, n2;
    int n11, n12, n13, n14, n21, n22, n23, n24;
    scanf("%d%d", &n1, &n2);
    n11 = n1 / 1000;
    n12 = ( n1 % 1000 ) / 100; 
    n13 = ( n1 % 100 ) / 10;
    n14 = n1 % 10;
    n21 = n2 / 1000; 
    n22 = ( n2 % 1000 ) / 100; 
    n23 = ( n2 % 100 ) / 10; 
    n24 = n2 % 10;
    if(n11 == n21) a++;
    if(n12 == n22) a++;
    if(n13 == n23) a++;
    if(n14 == n24) a++;
    if(n11 == n22) b++;
    if(n11 == n23) b++;
    if(n11 == n24) b++;
    if(n12 == n21) b++;
    if(n12 == n23) b++;
    if(n12 == n24) b++;
    if(n13 == n21) b++;
    if(n13 == n22) b++;
    if(n13 == n24) b++;
    if(n14 == n21) b++;
    if(n14 == n22) b++;
    if(n14 == n23) b++;
    printf("%dA%dB\n", a, b);
}