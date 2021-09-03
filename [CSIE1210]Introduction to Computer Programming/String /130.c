#include <stdio.h>
#include <string.h>
/*main*/
#define STRINGLEN 80
int main(void)
{
    int n, i;
    scanf("%d", &n);
    char str1[n][STRINGLEN], str2[n][STRINGLEN];
    int truefalse[n];
    for(i = 0; i < n; i++)
    {
        scanf("%s%s", str1[i], str2[i]);
        char *strptr1 = str1[i], *strptr2 = str2[i];
        if(strcmp(str1[i], str2[i]) == 0)
            truefalse[i] = 1;
        else if(strlen(str1[i]) == strlen(str2[i]))
        {
            int j, switchtime = 0;
            char temp;
            for(j = 0; j < strlen(str1[i]) - 1; j++)
            {
                temp = str1[i][j];
                str1[i][j] = str1[i][j + 1];
                str1[i][j + 1] = temp;
                if(strcmp(str1[i], str2[i]) == 0)
                    switchtime++;
                if(switchtime == 2)
                    break;
                temp = str1[i][j];
                str1[i][j] = str1[i][j + 1];
                str1[i][j + 1] = temp;
            }
            if(switchtime == 1)
                truefalse[i] = 1;
            else
            {
                truefalse[i] = 0;
                continue;
            }
        }
        else if((strlen(str1[i]) == strlen(str2[i]) + 1) || (strlen(str2[i]) == strlen(str1[i]) + 1))
        {
            char test1[STRINGLEN], test2[STRINGLEN], cmpstr[STRINGLEN];
            int j, checktime = 0;
            if(strlen(str1[i]) == strlen(str2[i]) + 1)
            {    
                strcpy(test1, str1[i]);
                strcpy(test2, str2[i]);
            }
            else
            {
                strcpy(test1, str2[i]);
                strcpy(test2, str1[i]);
            }
            for(j = 0; j < strlen(test1); j++)
            {
                strcpy(cmpstr, test1);
                strncpy(cmpstr, test1, j);
                cmpstr[j] = '\0';
                strcat(cmpstr, test1 + j + 1);
                if(strcmp(cmpstr, test2) == 0)
                {
                    truefalse[i] = 1;
                    checktime++;
                    break;
                }
            }
            if(checktime == 0)
            {
                truefalse[i] = 0;
                continue;
            } 
        }
        else
            truefalse[i] = 0;
    }
    for(i = 0; i < n; i++)
    {
        if (truefalse[i] == 1)
            printf("yes\n");
        else
            printf("no\n");
    }
    return 0;
}