#include <stdio.h>
int main(){
    int n, i, k;
    scanf("%d%d", &n, &i);
    switch(i){
    case 1:
        k = n;
        break;
    case 2:
        k = n * n;
        break;
    case 3:
        k = n * n * n;
        break;
    case 4:
        k = n * n * n * n;
        break;
    case 5:
        k = n * n * n * n * n;
    default:
        break;
    }
    printf("%d\n", k);
}