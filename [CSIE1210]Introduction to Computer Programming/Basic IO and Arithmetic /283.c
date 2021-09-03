#include <stdio.h>
int main(){
    int s, f, t, chick, rab, crab;
    scanf("%d%d%d", &s, &f, &t);
    crab = s - t; 
    rab = ((f - 8 * crab) / 2 - t);
    chick = t - rab;
    printf("%d\n%d\n%d\n", chick, rab, crab);
}