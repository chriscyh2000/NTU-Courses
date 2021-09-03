#include <stdio.h>
#include <stdbool.h>
int main(void){
    short R, C;
    scanf("%hd%hd", &R, &C);
    short arr[R][C]; 
    short c = 0;
    int count = 0;
    for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
            scanf("%hd", &arr[i][j]);
    for(int i = 1; i < R - 1; ++i){
        c += (arr[i][0] + arr[i][1] + arr[i][2]);
        for(int j = 0; j < C - 3; ++j){
            c += arr[i][j+3];
            if(arr[i][j] + arr[i][j+2] == 7 && arr[i][j+1] + arr[i][j+3] == 7){
                for(int k = 0; k < 4; ++k){
                    c += arr[i-1][j+k];
                    for(int l = 0; l < 4; ++l){
                        c += arr[i+1][j+l];
                        if(c == 21){
                            bool e[7] = {0};
                            e[arr[i+1][j+l]] = true;
                            e[arr[i-1][j+k]] = true;
                            e[arr[i][j]] = true;
                            e[arr[i][j+1]] = true;
                            e[arr[i][j+2]] = true;
                            e[arr[i][j+3]] = true;
                            if(e[1] && e[2] && e[3] && e[4] && e[5] && e[6])
                                ++count;
                            // printf("(%d, %d) (%d, %d) (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", i, j, i, j + 2, i, j + 1, i, j + 3, i - 1, j + k, i + 1, j + l);
                        }   
                        c -= arr[i+1][j+l];
                    }
                    c -= arr[i-1][j+k];
                }
            }
            c -= arr[i][j];
        }
        c = 0;
    }
    printf("%d\n", count);
}