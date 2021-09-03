#include <stdio.h>
int main(){
    int W, D;
    scanf("%d%d", &W, &D);
    int mirror[D][W];
    int i, j;
    for(i = D - 1; i >= 0; i--){
        for(j = 0; j < W; j++)
            scanf("%d", &mirror[i][j]);
    }
    int m;
    m = 2 * (W + D);
    int windowseenby[m];
    int strstep[m], parstep[m];
    int direction; //0代表左右，1代表上下
    for(i = 0; i < m; i++){
        strstep[i] = parstep[i] = 0;
    }
    for(i = W; i < 2 * W + D; i++){
        if((i >= W + D) && ( i <= 2 * W + D - 1)){
            direction = 1;
            while((parstep[i] != 2 * W + D - i) && (strstep[i] != D)){
                int y, x;
                y = D - 1 - strstep[i]; 
                x = 2 * W + D - i - 1 - parstep[i];
                if(mirror[y][x] == 1){
                    if(direction == 1){
                        parstep[i]++;
                        direction = 0;
                    }
                    else if(direction == 0){
                        strstep[i]++;
                        direction = 1;
                    }
                }
                else{
                    if(direction == 1){
                        strstep[i]++;
                        direction = 1;
                    }
                    else if(direction == 0){
                        parstep[i]++;
                        direction = 0;
                    }
                }  
            }
            if((parstep[i] == 2 * W + D - i) && (strstep[i] != D))
                windowseenby[i] = 2 * W + D + strstep[i];

            else if((parstep[i] != 2 * W + D - i) && (strstep[i] == D))
                windowseenby[i] = 2 * W + D - 1 - i - parstep[i];
        }
        if((i >= W) && (i <= W - 1 + D)){
            direction = 0;
            while((parstep[i] != W) && (strstep[i] != i - W + 1)){
                int y, x;
                y = i - W - strstep[i]; 
                x = W-1-parstep[i];
                if(mirror[y][x]==1){
                    if(direction == 1){
                        parstep[i]++;
                        direction = 0;
                    }
                    else if(direction == 0){
                        strstep[i]++;
                        direction = 1;
                    }
                }
                else{
                    if(direction == 1){
                        strstep[i]++;
                        direction = 1;
                    }
                    else if(direction == 0){
                        parstep[i]++;
                        direction = 0;
                    }
                }
            }
            if((parstep[i] == W) && (strstep[i] != i - W + 1))
                windowseenby[i] = 3 * W + 2 * D - i - 1 + strstep[i];
            else if((parstep[i] != W) && (strstep[i] == i - W + 1))
                windowseenby[i] = W - 1 - parstep[i];
        }
        for(j = 0; j < 2 * W + 2 * D; j++){
            if(windowseenby[i] == j)
                windowseenby[j] = i;
        }
    }
    for(i = 0; i < 2 * W + 2 * D; i++) 
        printf("%d\n", windowseenby[i]);
}