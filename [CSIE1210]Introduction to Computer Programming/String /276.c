#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* main */
#define STRLEN 150
int main(void)
{
    char inputstring[STRLEN], outputstring[40][STRLEN];
    int i = 0, k = 0;
    while(scanf("%s", inputstring) != EOF)
    {
        if(strcmp(inputstring, "of") == 0 || \
        strcmp(inputstring, "and") == 0 || \
        strcmp(inputstring, "the") == 0 || \
        strcmp(inputstring, "at") == 0) continue;
        if(strcmp(inputstring, "of.") == 0 || \
        strcmp(inputstring, "and.") == 0 || \
        strcmp(inputstring, "the.") == 0 || \
        strcmp(inputstring, "at.") == 0)
        {
            outputstring[i][k] = '\0';
            k = 0; 
            i++;
            continue;
        }
        outputstring[i][k] = toupper(inputstring[0]);
        k++;
        if(inputstring[strlen(inputstring) - 1] == '.')
        {
            outputstring[i][k] = '\0';
            k = 0; 
            i++;
        }
    }
    for(int j = 0; j < i; j++)
        printf("%s\n", outputstring[j]);
    return 0;
}