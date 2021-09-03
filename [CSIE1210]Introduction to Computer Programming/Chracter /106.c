#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    char c;
    int i, j, k = 0;
    int num;
    int n = 1;
    int calfor3[20000] = {0}, calfor11a[20000] = {0}, calfor11b[20000] = {0}, lastdigit[200000] = {0};
    while (scanf("%c", &c) != EOF)
    {
        if (c == '\n')
        {
            lastdigit[k] = num;
            k++;
            continue;
        }
        if (c == '-')
        {
            scanf("%c", &c);
            continue;
        }
        for (i = 0, j = 0; i <= 9; i++, j++)
        {
            if (c == '0' + i)
            {
                num = j;
                break;
            }
        }
        calfor3[k] += num;
        if(n % 2 == 0)
            calfor11a[k] += num;
        else
            calfor11b[k] += num;
        n++;
    }
    for ( i = 0; i < k - 1; i++)
    {
        if (lastdigit[i] % 2 == 0)
            printf("yes ");
        else
            printf("no ");
        if (calfor3[i] % 3 == 0)
            printf("yes ");
        else
            printf("no ");
        if ((lastdigit[i] == 5) ||(lastdigit[i] == 0))
            printf("yes ");
        else
            printf("no ");
        if (abs(calfor11b[i] - calfor11a[i]) % 11 == 0)
            printf("yes\n");
        else
            printf("no\n");
    }
    return 0;
}