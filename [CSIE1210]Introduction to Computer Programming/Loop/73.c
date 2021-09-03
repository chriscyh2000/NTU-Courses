#include <stdio.h>
int main(){
    int n, i = 0, j = 0, k = 0, p, count;
    scanf("%d", &n);
    for(count = 1; count <= n; ++count){
        scanf("%d", &p);
        if(p % 3 == 0) ++i;
        if(p % 3 == 1) ++j;
        if(p % 3 == 2) ++k;
    }
    printf("%d %d %d\n", i, j, k);
}