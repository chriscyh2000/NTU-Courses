#include <stdio.h>
int main(void)
{
    int k;
    scanf("%d", &k);
    int cp;
    int attribute[3] = {0};
    int sumofleader = 0;
    int member[200000];
    int numofmembers = 0;
    while(scanf("%d", &cp)!=EOF)
    {
        int atr;
        atr = cp % 3;
        if (atr == 0)
        {
            attribute[0]++;
            member[numofmembers] = cp; 
        }
        else if (atr == 1)
        {
            attribute[1]++;
            member[numofmembers] = cp; 
        }
        else
        {
            attribute[2]++;
            member[numofmembers] = cp;     
        }
        numofmembers++;
        if ((attribute[0] >= k) && (attribute[1] >= k) && (attribute[2] >= k))
        {
            int i, j;
            for (i = numofmembers-2; i >= 0; i--)
            {
                for (j = 0; j <= i; j++)
                {
                    if (member[j]>member[j+1])
                    {
                        int temp;
                        temp = member[j];
                        member[j] = member[j+1];
                        member[j+1] = temp;
                    }
                }
            }
            sumofleader += member[numofmembers-2];
            attribute[0] = attribute[1] = attribute[2] = numofmembers = 0;
        }
    }
    if (((attribute[0] < k) || (attribute[1] < k) || (attribute[2] < k)) && (numofmembers != 0))
    {
        int i, j;
        if (numofmembers >= 2)
        {
            for (i = numofmembers-2; i >= 0; i--)
            {
                for (j = 0; j <= i; j++)
                {
                    if (member[j]>member[j+1])
                    {
                        int temp;
                        temp = member[j];
                        member[j] = member[j+1];
                        member[j+1] = temp;
                    }
                }
            }
            sumofleader += member[numofmembers-2];
        }
        else 
        {
            sumofleader += member[0];
        }
    }
    printf("%d\n", sumofleader);
    return 0;
}