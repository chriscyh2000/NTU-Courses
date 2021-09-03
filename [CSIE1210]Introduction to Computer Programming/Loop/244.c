#include <stdio.h>
int main(){
    int specialprizenum[4];
    int firstprizenum[4];
    int yournum;
    int prize=0;
    specialprizenum[0] = firstprizenum[0] = 0;
    int i;
    for(i = 1 ; i <= 3 ; i++)
        scanf("%d", &specialprizenum[i]);
    for(i = 1; i<=3; i++)
        scanf("%d", &firstprizenum[i]);
    while(scanf("%d", &yournum) != EOF){
        if((yournum == specialprizenum[1]) || (yournum == specialprizenum[2]) || (yournum == specialprizenum[3]))
            prize += 2000000;
        else{
            if((yournum == firstprizenum[1]) || (yournum == firstprizenum[2]) || (yournum == firstprizenum[3]))
                prize += 200000;
            else if ((yournum % 10000000 == firstprizenum[1] % 10000000) || (yournum % 10000000 == firstprizenum[2] % 10000000) \
            || (yournum % 10000000 == firstprizenum[3] % 10000000))
                   prize += 40000;
            else if((yournum % 1000000 == firstprizenum[1] % 1000000) || (yournum % 1000000 == firstprizenum[2] % 1000000) \
            || (yournum % 1000000 == firstprizenum[3] % 1000000))
                prize += 10000;
            else if((yournum % 100000 == firstprizenum[1] % 100000) || (yournum % 100000 == firstprizenum[2] % 100000) \
            || (yournum % 100000 == firstprizenum[3] % 100000))
                prize += 4000;
            else if((yournum % 10000 == firstprizenum[1] % 10000) || (yournum % 10000 == firstprizenum[2] % 10000) \
            || ( yournum % 10000 == firstprizenum[3] % 10000))
                prize += 1000;
            else if((yournum % 1000 == firstprizenum[1] % 1000) || (yournum % 1000 == firstprizenum[2] % 1000) \
            || ( yournum % 1000 == firstprizenum[3] % 1000))
                prize += 200;  
        }
    }
    printf("%d\n", prize);
}
