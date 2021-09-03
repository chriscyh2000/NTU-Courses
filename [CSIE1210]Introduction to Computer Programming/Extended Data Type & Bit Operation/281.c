#include <stdio.h>
#define TOTALNUM 10000
int readNumber(int number, int bit[])
{
    int i = 0;
    while(scanf("%d", &(number)) != EOF)
    {
        for(int j = 0; j < sizeof(number) * 8; j++)
        {
            if((1 & number) != 0)
                bit[i]++;
            number >>= 1;
        }
        i++;
    }
    return i;
}
int main(void)
{
    int number, bit[TOTALNUM] = {0};
    int printNum = readNumber(number, bit);
    for(int k = 0; k < printNum; k++)
        printf("%d\n", bit[k]);
    return 0;
}