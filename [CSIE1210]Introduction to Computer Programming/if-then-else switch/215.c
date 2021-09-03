#include <stdio.h>
int main(){
    int s, f, t, chick, rab, crab;
    scanf("%d%d%d", &s, &f, &t);
    crab = s - t; 
    rab = ((f - 8 * crab) / 2 - t);
    chick = t - rab;
 
    if((crab < 0) || (rab < 0) || (chick < 0)) printf("0\n");
    else if(( f - 8 * crab ) % 2 != 0) printf("0\n");
    else printf("%d\n%d\n%d\n", chick, rab, crab);
}