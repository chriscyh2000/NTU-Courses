#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR 40
#define MAXSTRING 65
int compare(const void *data1, const void *data2)
{
    char *str1 = *(char **)data1;
    char *str2 = *(char **)data2;
    return strcmp(str1, str2);
}
int main(void)
{
    char *string[MAXSTRING], str[MAXSTRING][MAXCHAR];
    int i = 0;
    while((string[i] = fgets(str[i], MAXCHAR, stdin)) != NULL)
        i++;
    qsort(string, i, sizeof(char *), compare);
    for(int j = 0; j < i; j++)
        printf("%s", string[j]);
    return 0;
}