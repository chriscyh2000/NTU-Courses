#include <stdio.h>
int main(){
    int i, k, n;
    scanf("%d", &i);
    for(k = 0; k < i ; ++k){
        n = i;
        n -= k;
        printf("%d\n", n);
    }
}




