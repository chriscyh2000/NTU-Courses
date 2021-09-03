#include <stdio.h>
int main(void)
{
    int R, H;
    int g;
    scanf("%d%d%d", &R, &H, &g);
    int r = 0;
    int rprime[4] = {0};
    int base[4], cooldown[4], extra[4];
    cooldown[0] = 0; extra[0] = 0; 
    int i;
    for(i = 3; i >= 1; i--)
        scanf("%d%d%d", &base[i], &extra[i], &cooldown[i]);
    scanf("%d", &base[0]);
    int attack[4];
    attack[0] = base[0];
    int dragonblood = H;
    int times[4] = {0};
    while(r < R)
    {
        for (i = 3; i >= 1; i--)
        {
            if (times[i] == 0)
            {
                if(r <= cooldown[i])
                {
                    attack[i] = base[i];
                }
                else
                {
                    attack[i] = base[i] + extra[i] * (r - cooldown[i]);
                }
            }
            else
            {
                if (r - rprime[i] >= cooldown[i])
                    attack[i] = base[i] + extra[i] * (r - rprime[i] - cooldown[i]);
                else
                    attack[i] = 0;
            }    
        }
        int maxdamage = -1;
        int maxskill;
        for(i = 3; i >= 0; i--)
        {
            if(maxdamage < attack[i])
            {
                maxdamage = attack[i];
                maxskill = i;
            }
        }
        for(i = 3; i >= 1; i--)
        {
            if (i == maxskill)
            {
                rprime[i] = r;
                times[i]++; 
            }
        }

        dragonblood -= maxdamage;
        if (dragonblood <= 0)
        {
            printf("%d\n", dragonblood);
            break;
        }
        else
        {
            if(r < R -1)
                printf("%d ", dragonblood);
            else
                printf("%d\n", dragonblood);
        }
        if(dragonblood + g > H)
            dragonblood = H;
        else
            dragonblood += g;
        r++;
    }
    return 0;
}