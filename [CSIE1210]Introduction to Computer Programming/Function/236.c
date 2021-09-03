#include <stdio.h>
int gcd(int i, int j);
int main(void)
{
    int num;
    int lcm = 1;
    while (scanf("%d", &num) != EOF)
    {
        lcm = lcm * num / gcd(lcm, num);
    }
    printf("%d\n", lcm);
    return 0;
}
int gcd (int i, int j)
{
    int temp;
    int ans;
    if (i < j)
    {
        temp = i;
        i = j; j = temp;
    }
    while (i % j != 0)
    {
        temp = i % j;
        i = j;
        j = temp;
    }
    return j;
}