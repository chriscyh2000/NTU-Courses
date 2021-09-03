#include <stdio.h>
int main(void)
{
    char c;
    int k = 0;
    int numofsentence = 0;
    int continuedot = 1;
    char cprime = ' ', tempc;
    while(scanf("%c", &c) != EOF)
    {
        if ((cprime == c) && (c == '.'))
            continuedot++;
        else if(c == '\n')
        {}
        else
            continuedot = 1;
        cprime = c;
        if(c != '.')
            continue;
        if(c == '.')
        {
            scanf("%c", &c);
            if ((cprime == c) && (c == '.'))
                continuedot++;
            else
                continuedot = 1;
            cprime = c;
            if ((c == '\n') || (c == '\0'))
            {
                numofsentence++;
                continue;
            }
            if (c == ' ')
            {
                scanf("%c", &c);
                if ((cprime == c) && (c == '.'))
                    continuedot++;
                else
                    continuedot = 1;
                cprime = c;
                if (c == ' ')
                {
                    numofsentence++;
                    continue;
                }
            }
            while((c == '\n') || (c == ' '))
            {
                scanf("%c", &c);
                if ((cprime == c) && (c == '.'))
                    continuedot++;
                else
                    continuedot = 1;
                cprime = c;
                if (c == '\0')
                {
                    numofsentence++;
                    break;
                }
                else if((c == '\n') || (c == ' '))
                    continue;
            }
        }
    }
    if (continuedot % 2 == 0)
        numofsentence++;
    printf("%d\n", numofsentence);
    return 0;
}