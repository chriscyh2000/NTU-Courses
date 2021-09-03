#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define KINDS 3
bool makeLotion(int desiredPower, int unit[3], int power[3])
{
    if (desiredPower == 0)
        return true;
    for(int i = 0; i < KINDS; i++)
    {
        if (desiredPower - power[i] >= 0 && unit[i] > 0)
        {
            desiredPower -= power[i];
            unit[i]--; 
            if(makeLotion(desiredPower, unit, power))
                return true;
            desiredPower += power[i];
            unit[i]++;
        }        
    }
    return false;
}
int main(void)
{
    int N;
    scanf("%d", &N);
    char YesOrNo[N][4];
    for(int i = 0; i < N; i++)
    {
        int desiredPower, unit[3], power[3];
        scanf("%d", &desiredPower);
        scanf("%d%d%d", &unit[0], &unit[1], &unit[2]);
        scanf("%d%d%d", &power[0], &power[1], &power[2]);
        if(makeLotion(desiredPower, unit, power)) strcpy(YesOrNo[i], "yes");
        else strcpy(YesOrNo[i], "no");
    }
    for(int i = 0; i< N; i++)
        printf("%s\n", YesOrNo[i]);
    return 0;
}