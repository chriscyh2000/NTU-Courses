#include <stdio.h>
int main()
{
    int n;
    scanf("%d", &n);
    int integersequence[n];
    int odd[1000] = {0};
    int even[1000] = {0};
    int oddnum = 0, evennum = 0;
    int i; 
    for (i = 0; i < n; i++)
    {   
        scanf("%d", &integersequence[i]);
        if (integersequence[i] % 2 != 0)
        {
            odd[oddnum] = integersequence[i];
            oddnum++;
        }
        else
        {
            even[evennum] = integersequence[i];
            evennum++;
        }
    }
    for (i = 0; i < oddnum - 1; i++)
    {
        printf("%d ", odd[i]);
    }
    printf("%d\n", odd[oddnum - 1]);
    for (i = 0; i < evennum - 1; i++)
    {
        printf("%d ", even[i]);
    }
    printf("%d\n", even[evennum - 1]);
}