#include <stdio.h>
int squareSum(int n)
{
    if(n == 1)
        return 1;
    return (n * n + squareSum(n - 1));
}
int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%d\n", squareSum(n));
    return 0;
}