#include <stdio.h>
long int sqr(long int num);
int main(void){
    int n;
    scanf("%d", &n);
    int x[n][3], y[n][3], r[n][3];
    int i, j;
    long int p, q;
    int total[n];
    for(i = 0; i < n; i++){
        long int minxin[3], minyin[3], maxxin[3], maxyin[3];
        int region[7] = {0};
 
        for(j = 0; j < 3; j++){
            scanf("%d%d%d", &x[i][j], &y[i][j], &r[i][j]);
            minxin[j] = x[i][j] - r[i][j];
            maxxin[j] = x[i][j] + r[i][j];
            minyin[j] = y[i][j] - r[i][j];
            maxyin[j] = y[i][j] + r[i][j];
            for(p = minxin[j]; p <= maxxin[j]; p++){
                for(q = minyin[j]; q <= maxyin[j]; q++){
                    if(sqr(p-x[i][j])+sqr(q-y[i][j]) <= sqr(r[i][j]))
                        region[j]++;
                }
            }
        }
 
        for(p = minxin[0]; p <= maxxin[0]; p++){
            for(q = minyin[0]; q <= maxyin[0]; q++){
                if((sqr(p-x[i][0])+sqr(q-y[i][0]) <= sqr(r[i][0]))//
                &&(sqr(p-x[i][1])+sqr(q-y[i][1]) <= sqr(r[i][1])))
                    region[3]++;
                if((sqr(p-x[i][0])+sqr(q-y[i][0]) <= sqr(r[i][0]))//
                &&(sqr(p-x[i][2])+sqr(q-y[i][2]) <= sqr(r[i][2])))
                    region[4]++;
                if((sqr(p-x[i][0])+sqr(q-y[i][0]) <= sqr(r[i][0]))//
                &&(sqr(p-x[i][1])+sqr(q-y[i][1]) <= sqr(r[i][1]))//
                &&(sqr(p-x[i][2])+sqr(q-y[i][2]) <= sqr(r[i][2])))
                    region[6]++;
            }
        }
 
        for(p = minxin[1]; p <= maxxin[1]; p++){
            for(q = minyin[1]; q <= maxyin[1]; q++){
                if((sqr(p-x[i][1])+sqr(q-y[i][1]) <= sqr(r[i][1]))//
                &&(sqr(p-x[i][2])+sqr(q-y[i][2]) <= sqr(r[i][2])))
                    region[5]++;
            }
        }
        total[i] = region[0] + region[1] + region[2] - 2 * region[3] - 2 * region[4] - 2 * region[5] + 4 * region[6];
    } 
    for(i = 0; i < n; ++i) printf("%d\n", total[i]);
    return 0;
}
long int sqr(long int num){
    long int ans;
    ans = num*num;
    return ans;
}