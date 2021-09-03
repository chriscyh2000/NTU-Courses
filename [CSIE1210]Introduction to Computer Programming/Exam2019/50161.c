#include <stdio.h>
int main(void)
{
    int N;
    scanf("%d", &N);
    int cardvalue[N];
    int valueindex[101];
    int i, j, temp;
    for(i = 0; i < 101; i++)
        valueindex[i] = -1;
    for (i = 0; i < N; i++)
    {
        scanf("%d", &cardvalue[i]);
        printf("%d", i);
        temp = cardvalue[i];
        if (valueindex[temp] != -1)
        {
            printf(" %d %d", valueindex[temp], cardvalue[i]);
            valueindex[temp] = -1;
        }
        else
        {
            valueindex[temp] = i;
        }
        printf("\n");
    }
    return 0;
}