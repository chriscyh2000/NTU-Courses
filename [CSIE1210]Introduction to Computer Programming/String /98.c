#include <stdio.h>
#include <string.h>
#define INGREKIND 20
#define NAMELENGTH 70
int main(void)
{
    int n, m;
    int i, j, k;
    scanf("%d", &n);
    char food[n][NAMELENGTH], ingredient[n][INGREKIND][NAMELENGTH];
    int sorts[n];
    for(j = 0; j < n; j++)
    {
        scanf("%s%d", food[j], &sorts[j]);
        for(k = 0; k < sorts[j]; k++)
            scanf("%s", ingredient[j][k]);
    }
    scanf("%d", &m);
    char compare1[m][NAMELENGTH], compare2[m][NAMELENGTH];
    int index1[m], index2[m];
    char common[INGREKIND][NAMELENGTH], *comptr[INGREKIND], temp[NAMELENGTH];
    int count = 0;
    for(j = 0; j < m; j++)
    {
        scanf("%s%s", compare1[j], compare2[j]);
        for(k = 0; k < n; k++)
        {
            if(strcmp(compare1[j], food[k]) == 0)
                index1[j] = k;
            if(strcmp(compare2[j], food[k]) == 0)
                index2[j] = k;
        }
    }
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < sorts[index1[i]]; j++)
        {
            for(k = 0; k < sorts[index2[i]]; k++)
            {
                if(strcmp(ingredient[index1[i]][j], ingredient[index2[i]][k]) == 0)
                {
                    strcpy(common[count], ingredient[index1[i]][j]);
                    count++;
                    break;
                }
            }
        }
        if(count > 0)
        {
            for(j = count - 2; j >= 0; j--)
            {
                for(k = 0; k <= j; k++)
                {
                    if(strcmp(common[k], common[k + 1]) > 0)
                    {
                        strcpy(temp, common[k]);
                        strcpy(common[k], common[k + 1]);
                        strcpy(common[k + 1], temp);
                    }
                }
            }
            printf("%s", common[0]);
            for(j = 1; j < count; j++)
                printf(" %s", common[j]);
            printf("\n");
        }
        else
            printf("nothing\n");
        count = 0;
    }
    return 0;
}