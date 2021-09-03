#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    int integersequence[n];
    int i;
    for (i = 0; i < n; i++)
    {
        scanf("%d", &integersequence[i]);
    }
    for (i = n - 1; i > 0; i--)
    {
        printf("%d ", integersequence[i]);
    }
    printf("%d\n", integersequence[i]);
}