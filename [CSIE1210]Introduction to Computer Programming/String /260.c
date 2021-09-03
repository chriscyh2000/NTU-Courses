#include <stdio.h>
#include <string.h>
/* main */
#define STRLEN 128
int main(void)
{
    char output[STRLEN], temp[STRLEN];
    scanf("%s", output);
    int testlength;
    while(scanf("%s", temp) != EOF)
    {
        if(strlen(output) > strlen(temp))
            testlength = strlen(output);
        else
            testlength = strlen(temp);
        if(strcmp(output, temp) == 0)
            continue;
        else
        {
            for(int i = testlength; i > 0; i--)
            {
                if(strncmp(&output[strlen(output) - i], temp, i) == 0)
                {
                    output[strlen(output) - i] = '\0';
                    break;
                }
            }
            strcat(output, temp);
        }
    }
    printf("%s\n", output);
    return 0;
}