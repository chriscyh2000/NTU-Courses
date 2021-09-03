#include <stdio.h>
#include <stdlib.h>
int main(){
    int y, m, d, firstday, n, i;
    scanf("%d%d", &y, &firstday);
    int day[13];
    int total[11], today[11];

    day[0] = 0;
    if((y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0)))
        day[2]=29;
    else 
        day[2]=28;
    day[1] = day[3] = day[5] = day[7] = day[8] = day[10] = day[12] = 31;
    day[4] = day[6] = day[9] = day[11] = 30;
    for(i = 0; i <= 10; ++i){
        total[i] = 0;
        today[i] = 0;
    }
    scanf("%d", &n);
    for (i = 1; i <= n; ++i){
        scanf("%d%d", &m, &d);
        switch (m){
            case 1:{    
                total[i] = d;
                break;
            }
            case 2:{
                total[i] = day[1] + d;
                break;
            }
            case 3:{
                total[i] = day[1] + day[2] + d;
                break;
            }
            case 4:{
                total[i] = day[1] + day[2] + day[3] + d;
                break;
            }
            case 5:{
                total[i] = day[1] + day[2] + day[3] + day[4] + d;
                break;
            }
            case 6:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + d;
                break;
            }
            case 7:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + day[6] + d;
                break;
            }
            case 8:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + day[6] + day[7] + d;
                break;
            }
            case 9:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + day[6] + day[7] + day[8] + d;
                break;
            }
            case 10:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + day[6] + day[7] + day[8] + day[9] + d;
                break;
            }
            case 11:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + day[6] + day[7] + day[8] + day[9] + day[10] + d;
                break;
            }
            case 12:{
                total[i] = day[1] + day[2] + day[3] + day[4] + day[5] + day[6] + day[7] + day[8] + day[9] + day[10] + day[11] + d;
                break;
            }
            default:{    
                total[i] = 0;
                break;
            }
        }
        today[i] = (firstday + (total[i] % 7) - 1) % 7;
        if ((m > 12) || (m <= 0))
            today[i]=-1;
        else{
            if((d > day[m]) || (d <= 0))
                today[i] = -2;   
        }
    }
    for (i = 1; i <= n; ++i)
        printf("%d\n", today[i]);
}