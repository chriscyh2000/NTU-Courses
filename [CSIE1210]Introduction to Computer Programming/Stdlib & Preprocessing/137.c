#include <stdio.h>
#include <string.h>


int main(void)
{
    int grade[1000];
    int n = 0;
    while(scanf("%d", &grade[n]) != EOF)
        n++;
    #ifdef HUNDRED
    int total = 0;  
    float avr;
    for(int i = 0; i < n; i++)
        total += grade[i];
    avr = ((float)total / n); 
    for(int i = 0; i < n; i++)
        printf("%d ", grade[i]);
    printf("%.1f\n", avr);
    #endif
    #ifdef APLUS
    #define CODE 
    char rank[n][4];
    int total = 0;
    float totalgpa = 0;
    float avr, avrgpa;
    for(int i = 0; i < n; i++)
    {
        if(grade[i] >= 90 && grade[i] <= 100)
        {
            strcpy(rank[i], "A+");
            total += 95;
            totalgpa += 4.3;
        }
        else if(grade[i] >= 85 && grade[i] <= 89)
        {   
            strcpy(rank[i], "A");  
            total += 87;
            totalgpa += 4.0;
        }
        else if(grade[i] >= 80 && grade[i] <= 84)
        {
            strcpy(rank[i], "A-");  
            total += 82;
            totalgpa += 3.7;
        }
        else if(grade[i] >= 77 && grade[i] <= 79)
        {
            strcpy(rank[i], "B+");
            total += 78;
            totalgpa += 3.3;
        }
        else if(grade[i] >= 73 && grade[i] <= 76)
        {
            strcpy(rank[i], "B");
            total += 75;
            totalgpa += 3.0;
        }
        else if(grade[i] >= 70 && grade[i] <= 72)
        {
            strcpy(rank[i], "B-");
            total += 70;
            totalgpa += 2.7;
        }
        else if(grade[i] >= 67 && grade[i] <= 69)
        {
            strcpy(rank[i], "C+");
            total += 68;
            totalgpa += 2.3;
        }
        else if(grade[i] >= 63 && grade[i] <= 66)
        {
            strcpy(rank[i], "C");
            total += 65;
            totalgpa += 2.0;
        }
        else if(grade[i] >= 60 && grade[i] <= 62)
        {
            strcpy(rank[i], "C-");
            total += 60;
            totalgpa += 1.7;
        }
        else if(grade[i] >= 0 && grade[i] <= 59)
        {
            strcpy(rank[i], "F");
            total += 50;
            totalgpa += 0;
        }
    }
    avr = (float)total/ n; avrgpa = totalgpa / n;  
    for(int j = 0; j < n; j++)
        printf("%s ", rank[j]);
    printf("%.1f %.1f\n", avrgpa, avr);
    #endif
    return 0;
}