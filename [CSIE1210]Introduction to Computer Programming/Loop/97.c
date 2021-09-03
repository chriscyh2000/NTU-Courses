#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int main(){
    int n, m;
    int parkinglot[11][4];
    scanf("%d", &n);
    int i;
    for(i = 0; i <= 3; ++i)
        parkinglot[0][i] = 0;
    for(i = 1; i <= n; ++i){
        scanf("%d%d%d", &parkinglot[i][0], &parkinglot[i][1], &parkinglot[i][2]);
        parkinglot[i][3]=0;
    }
    int j, X, Y;
    scanf("%d",&m);
    for (j = 1 ; j <= m; ++j){
        scanf("%d%d", &X, &Y);
        int min = 40000, minX, minY, minP;
        for (i = 1; i <= n; ++i){
            if(parkinglot[i][2] == 0) continue;
            else{
                if(min > (abs(X - parkinglot[i][0]) + abs(Y - parkinglot[i][1]))){
                    min = (abs(X - parkinglot[i][0]) + abs(Y - parkinglot[i][1]));
                    minX = parkinglot[i][0]; 
                    minY = parkinglot[i][1]; 
                    minP = i;
                }
                else if(min == (abs(X - parkinglot[i][0]) + abs(Y - parkinglot[i][1]))){
                    if(minX > parkinglot[i][0]){
                        minX = parkinglot[i][0]; 
                        minP = i;
                    }
                    else if(minX == parkinglot[i][0]){
                        if ( minY > parkinglot[i][1] ){
                            minY = parkinglot[i][0];
                            minP = i;
                        }
                    }        
                }
            }
        }
        parkinglot[minP][2]--;
        parkinglot[minP][3]++;
    }
    for (i = 1; i <= n; ++i)
        printf("%d\n",parkinglot[i][3]);
}