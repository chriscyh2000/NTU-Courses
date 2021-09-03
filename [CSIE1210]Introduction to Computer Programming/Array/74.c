#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    int i;
    int num[n];
    int remaindersum[200000] = {0}, remainder;
    for (i = 0; i < n; i++)
    {
        scanf("%d", &num[i]);
    }
    int m;
    scanf("%d", &m);
    for (i = 0; i < n; i++)
    {
        remainder = num[i] % m;
        remaindersum[remainder]++;
    }
    for (i = 0; i < m; i++)
    {
        printf("%d\n", remaindersum[i]);
    }
}