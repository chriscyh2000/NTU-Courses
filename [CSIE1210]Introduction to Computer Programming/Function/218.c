#include <stdio.h>
#include <assert.h>
long int c(int i, int j);
int main(void)
{
    int n, m;
    scanf("%d%d", &n, &m);
    assert((m <= n) && (m >= 0)) ;
    assert((n > 0) && (n < 15));
    long int sum = 0;
    for(int i = 0; i <= m; i++)
        sum += c(n, i);
    printf("%ld\n", sum);
    return 0;
}
long int c(int i, int j)
{
    long int answer = 1;
    int k;
    for(k = j + 1; k <= i; k++)
        answer *= k;
    for(k = 1; k <= i - j; k++)
        answer /= k;
    return answer;
}