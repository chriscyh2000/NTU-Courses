#include <stdio.h>
int main(){
    int i, j, k, n;
    scanf("%d", &i);
    for(j = 1; j <= i; ++j){
        printf("%d\n", j);
        if(j == i){
            for(k = 1; k < i; ++k){
                n = i;
                n -= k;
                printf("%d\n", n);
            }
        }
    }
}