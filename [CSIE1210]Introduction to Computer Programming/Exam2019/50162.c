#include <stdio.h>
#define WAITLISTSIZE 5000
int main(void)
{
    int classIndex;
    int DPS[WAITLISTSIZE] = {0}, Healer[WAITLISTSIZE] = {0}, Tank[WAITLISTSIZE] = {0};
    int remainDPS = 0, remainHealer = 0, remainTank = 0;
    int front[3] = {0};
    int i = 0, j = 0, k = 0;
    int teammates[5];
    while(scanf("%d", &classIndex) != EOF)
    {
        if(classIndex % 3 == 0)
        {
            DPS[i] =  classIndex;
            i = (i + 1 == 5000) ? 0 : (i + 1);
            remainDPS++;
        }
        else if(classIndex % 3 == 1)
        {
            Healer[j] = classIndex;
            j = (j + 1 == 5000) ? 0 : (j + 1);
            remainHealer++;
        }
        else 
        {
            Tank[k] = classIndex;
            k = (k + 1 == 5000) ? 0 : (k + 1);
            remainTank++;
        }
        if((remainDPS >= 3) && (remainHealer >= 1) && (remainTank >= 1))
        {
            remainDPS -= 3; remainHealer--; remainTank--;
            teammates[0] = DPS[front[0]];
            if(front[0] + 1 >= 5000)
                teammates[1] = DPS[front[0] + 1 - 5000];
            else
                teammates[1] = DPS[front[0] + 1];
            if(front[0] + 2 >= 5000)
                teammates[2] = DPS[front[0] + 2 - 5000];
            else
                teammates[2] = DPS[front[0] + 2];
            teammates[3] = Healer[front[1]];
            teammates[4] = Tank[front[2]];
            if(front[0] + 3 >= 5000)
                front[0] = front[0] + 3 - 5000;
            else
                front[0] = front[0] + 3;
            if(front[1] + 1 == 5000)
                front[1] = 0;
            else
                front[1]++;
            if(front[2] + 1 == 5000)
                front[2] = 0;
            else
                front[2]++;
        }
        else continue;
        for(int n = 0; n < 5; n++)
        {
            if(n != 4)  printf("%d ", teammates[n]);
            else    printf("%d\n", teammates[n]);
        }
    }
    return 0;
}