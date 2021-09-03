#include <stdio.h>
#include <ctype.h>
#define ALPHABET 26
int main(void)
{
    char c;
    int i;
    int appeartimes[ALPHABET] = {0};
    while(scanf("%c", &c) != EOF)
    {
        for (i = 97; i <= 122; i++)
        {
            c = tolower(c);
            if (c == i)
            {
                appeartimes[i-97]++;
                break;
            }
        }
    }
    for(i = 0; i < ALPHABET; i++)
        printf("%d\n", appeartimes[i]);
    return 0;
}