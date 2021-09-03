#include <stdio.h>
int main(void)
{
    char digit[10][11], SPACE;
    int length[10], height[10];
    int digitnum[10];
    int i = 0, j, l, h, k = 0, p;
    while(scanf("%c", &digit[k][i]) != EOF)
    {
        if (digit[k][i] == ' ')
        {
            scanf("%d %d", &length[k], &height[k]);
            scanf("%c", &SPACE);
            digitnum[k] = i;
            i = 0;
            k++;
            continue;
        }
        i++;
    }
    for(p = 0; p < k; p++)
    {
        for(h = 1; h <= height[p]; h++)
        {
            for(j = 0; j < digitnum[p]; j++)
            {
                    if(digit[p][j] == '0')
                    {
                        if((h == 1) || (h == height[p]))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if (h == (height[p] + 1) / 2)
                        {
                            for (l = 1; l <= length[p]; l++)
                                printf(" ");
                        }
                        else
                        {
                            printf("%c", digit[p][j]);
                            for(l = 2; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '1')
                    {
                        if ((h == 1) || (h == height[p]) || (h == (height[p] + 1) / 2))
                        {
                            for(l = 1; l <= length[p]; l++)
                                printf(" ");
                        }
                        else 
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '2')
                    {
                        if ((h == 1) || (h == height[p]) || (h == (height[p] + 1) / 2))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if ((h > 1) && (h < (height[p] + 1) / 2))
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                        else
                        {
                            printf("%c", digit[p][j]);
                            for (l = 2; l <= length[p]; l++)
                                printf(" ");
                        }
                    }
                    else if(digit[p][j] == '3')
                    {
                        if ((h == 1) || (h == height[p]) || (h == (height[p] + 1) / 2))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '4')
                    {
                        if ((h == 1) || (h == height[p]))
                        {
                            for (l = 1; l <= length[p]; l++)
                            {
                                printf(" ");
                            }
                        }
                        else if (h == (height[p] + 1) / 2)
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if ((h > 1) && (h < (height[p] + 1) / 2))
                        {
                            printf("%c", digit[p][j]);
                            for(l = 2; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                        else
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '5')
                    {
                        if ((h == 1) || (h == height[p]) || (h == (height[p] + 1) / 2))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if ((h > 1) && (h < (height[p] + 1) / 2))
                        {
                            printf("%c", digit[p][j]);
                            for (l = 2; l <= length[p]; l++)
                                printf(" ");
                        }
                        else
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '6')
                    {
                        if ((h == 1) || (h == height[p]) || (h == (height[p] + 1) / 2))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if ((h > 1) && (h < (height[p] + 1) / 2))
                        {
                            printf("%c", digit[p][j]);
                            for (l = 2; l <= length[p]; l++)
                                printf(" ");
                        }
                        else 
                        {
                            printf("%c", digit[p][j]);
                            for(l = 2; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '7')
                    {
                        if ((h > 1) && (h < (height[p] + 1) / 2))
                        {
                            printf("%c", digit[p][j]);
                            for(l = 2; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                        else if (h == 1)
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if ((h == (height[p] + 1) / 2) || (h == height[p]))
                        {
                            for (l = 1; l <= length[p]; l++)
                                printf(" ");
                        }
                        else
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '8')
                    {
                        if ((h == 1) || (h == height[p]) || (h == (height[p] + 1) / 2))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else
                        {
                            printf("%c", digit[p][j]);
                            for(l = 2; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
                    else if(digit[p][j] == '9')
                    {
                        if ((h == (height[p] + 1) / 2) || (h == 1))
                        {
                            printf(" ");
                            for(l = 2; l <= length[p] - 2; l++)
                                printf("%c", digit[p][j]);
                            printf(" ");
                            printf(" ");
                        }
                        else if ((h > 1) && (h < (height[p] + 1) / 2))
                        {
                            printf("%c", digit[p][j]);
                            for(l = 2; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                        else if (h == height[p])
                        {
                            for (l = 1; l <= length[p]; l++)
                                printf(" ");
                        }
                        else
                        {
                            for (l = 1; l <= length[p] - 2; l++)
                                printf(" ");
                            printf("%c", digit[p][j]);
                            printf(" ");
                        }
                    }
            }
                printf("\n");
        }
    }    
    return 0;
}