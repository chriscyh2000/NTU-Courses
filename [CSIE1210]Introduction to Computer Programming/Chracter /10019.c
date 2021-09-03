#include <stdio.h>
#include <ctype.h>
int main(void)
{
    char testc;
    char c[2][101];
    c[0][0] = c[1][0] = '.';
    int i = 1, j, k = 0, h;
    int contispace[2] = {1}, length[2] = {0};
    while(scanf("%c", &testc) != EOF)
    {
        while(isspace(testc))
        {
            scanf("%c", &testc);
        }
        while(testc != '\n')
        {
            c[k][i] = testc;
            if((c[k][i] == c[k][i-1]) && isspace(c[k][i]))
                contispace[k]++;
            else 
                contispace[k] = 1;
            i++;
            scanf("%c", &testc);
        }
        if (isspace(c[k][i-1]) == 0)
            contispace[k] = 0;
        length[k] = i;
        i = 1;
        k++;
    }
    for (h = 0; h < k; h++)
    {
        for(j = 1; j < length[h] - contispace[h]; j++)
            printf("%c", c[h][j]);
        printf("\n");
    }
    return 0;
}