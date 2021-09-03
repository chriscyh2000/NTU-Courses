#include <stdio.h>
#include <string.h>
int min = 200;
int minimum(int a, int b)
{
    return (a < b)? a : b;
}
int editDistance(char *str1, char *str2, int add)
{
    if((add > min) || (strcmp(str1, str2) == 0)) return add;
    int len1 = strlen(str1), len2 = strlen(str2);
    if(len1 == 0 || len2 == 0)
        return (add + len1 + len2);
    if(str1[0] == str2[0])
        return (editDistance(str1 + 1, str2 + 1, add));
    return (minimum(editDistance(str1 + 1, str2, add + 1),\
                        editDistance(str1, str2 + 1, add + 1)));
}
int main(void)
{
    char str[101][11];
    int i = 1;
    int index1 = 0, index2 = 0;
    while(scanf("%s", str[i]) != EOF) i++;
    for(int j = 1; j < i; j++)
    {
        for(int k = j + 1; k < i; k++)
        {
            int distance = editDistance(str[j], str[k], 0);
            if(distance < min ||\
               ((distance == min) &&\
                (1000 * j + k < 1000 * index1 + index2)))
            {
                index1 = j; index2 = k;
                min = distance;
            } 
        }
    }
    printf("%d %d %d\n", min, index1, index2);
    return 0;
}