#include <stdio.h>
int main(void)
{
    int n;
    scanf("%d", &n);
    double a[n][n], x[n], y[n];
    int i, j;
    for (i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            scanf("%lf", &(a[i][j]));
        }
    }
    for (i = 0; i < n; i++)
        scanf("%lf", &(y[i]));
    for (i = n - 1; i >= 0; i--)
    {
        x[i] = y[i];
        for (j = n - 1; j > i; j--)
        {
            x[i] -= a[i][j] * x[j] ;
        }
        x[i] /= a[i][i];
    }
    for (i = 0; i < n; i++)
        printf("%f\n", x[i]);
    return 0;
}