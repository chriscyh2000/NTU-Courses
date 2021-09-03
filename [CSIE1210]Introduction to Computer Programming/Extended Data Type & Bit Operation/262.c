#include<stdio.h>
int checkConsecutiveOf1(int number)
{
    int consecutives1 = 0;
    int temp = 0;
    for(int i = 0; i < sizeof(number) * 8; i++)
    {
        if((1 & number) != 0)
        {
            temp++;
        }
        else
            temp = 0;
        if(temp > consecutives1)
            consecutives1 = temp;
        number >>= 1;
    }
    return consecutives1;
}
void readNumandPrint(int number, int consecutive)
{
    while(scanf("%d", &(number)) != EOF)
        printf("%d\n", checkConsecutiveOf1(number));
}
int main(void)
{
    int number, consecutive;
    readNumandPrint(number, consecutive);
    return 0;
}