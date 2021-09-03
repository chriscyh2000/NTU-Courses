#include<stdio.h>
int checkNumberOf1(long long int number)
{
    int total = 0;
    int temp = 0;
    for(int i = 0; i < sizeof(number) * 8; i++)
    {
        if((1 & number) != 0)
            total++;
        number >>= 1;
    }
    return total;
}
void readNumandPrint(long long int number, int total)
{
    while(scanf("%lld", &(number)) != EOF) 
        printf("%d\n", checkNumberOf1(number));
}
int main(void)
{
    long long int number;
    int total;
    readNumandPrint(number, total);
    return 0;
}