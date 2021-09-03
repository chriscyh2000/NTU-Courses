#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int length, width;
    char one[2], zero[2];
    unsigned int number[1025];
    scanf("%d%d", &length, &width);
    scanf("%s%s", one, zero);
    int i = 0;
    while(scanf("%u", &number[i]) != EOF)
        i++;
    int index = 0;
    int j, k;
    for(j = 0; j < i; j++)
    {
        for(k = 31; k >= 0; k--)
        {
            if(index == length * width)
                break;
            if(((number[j] >> k) & 1) != 0)
                printf("%s", one);
            else
                printf("%s", zero);
            if(index % length == length - 1)
                printf("\n");
            index++;
        }
        if(index == length * width)
            break;
    }
    return 0;
}