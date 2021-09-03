#include <stdio.h>
int main(){
    int i, j, k;
    scanf("%d", &i);
    for(j = 1; j <= i ; ++j){
        for(k = 1 ;k <= j ; ++k)
            printf("%d", k);
    }
    printf("\n");
}