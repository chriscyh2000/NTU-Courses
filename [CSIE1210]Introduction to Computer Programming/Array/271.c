#include <stdio.h>
int main()
{
    int n, m;
    scanf("%d", &n);
    int a[n];
    int i, j, k;
    for (i = n - 1; i >= 0; i--)
    {
        scanf("%d", &a[i]);
    }
    scanf("%d", &m);
    int b[m];
    for (i = m - 1; i >= 0; i--)
    {
    scanf("%d", &b[i]);
    }
    int c[199] = {0};
    for (i = n -1; i >= 0; i--)
    {
        for (j = m - 1; j >= 0; j--)
        {
            c[i+j] += a[i] * b[j];
        }
    }
    for (i = n + m - 2; i >= 1; i--)
    {
        printf("%d ", c[i]);
    }
    printf("%d\n", c[0]);
}