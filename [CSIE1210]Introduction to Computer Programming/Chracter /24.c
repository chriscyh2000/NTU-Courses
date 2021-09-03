#include <stdio.h>
#include <ctype.h>
int main(void)
{
    char c;
    int numberoffigure = 0, numofalpha = 0, numofvowel = 0, numofconsonant = 0;
    while(scanf("%c", &c) != EOF)
    {
        c = tolower(c);
        if (c == '\n')
            continue;
        if (isalnum(c) && (isalpha(c) == 0))
            numberoffigure++;
        if (isalpha(c))
            numofalpha++;
        if ((c == 'a') || (c == 'e') || (c == 'i') || (c == 'o') || (c == 'u'))
            numofvowel++;
        else if(isalpha(c))
            numofconsonant++;
    }
    printf("%d %d %d %d\n", numberoffigure, numofalpha, numofvowel, numofconsonant);
    return 0;
}