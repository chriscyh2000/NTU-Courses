#include <stdio.h>
int main(){
    int h, w, d, surface, volumn;
    scanf("%d",&h);
    scanf("%d",&w);
    scanf("%d",&d);
    surface = h * d * 2 + h * w * 2 + w * d * 2;
    volumn = h * d * w;
    printf("%d\n", surface);
    printf("%d\n", volumn);

}