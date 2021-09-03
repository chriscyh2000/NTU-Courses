#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAXARRAY 1000000 
typedef struct date{
    int year, month, day;
} Date;
Date dates[MAXARRAY];
/* 所需陣列較大就要在main外面宣告才不會segmentation fault */
int compare(const void *data1, const void *data2)
{
    Date d1 = *(Date *)data1;
    Date d2 = *(Date *)data2;
    if(d1.year != d2.year)
        return (d1.year - d2.year);
    if(d1.month != d2.month)
        return (d1.month - d2.month);
    return (d1.day - d2.day);
    /* 若選擇使用指標去記(Date *)data1，
       然後用該指標->year去比大小、會造成所需時間較長導致TLE */
}
int main(void)
{
    int n, i;
    scanf("%d", &n);
    for(i = 0; i < n; i++)
        scanf("%d%d%d", &(dates[i].year), &(dates[i].month), &(dates[i].day));
    qsort(dates, n, sizeof(Date), compare);
    for(i = 0; i < n; i++)
        printf("%d %d %d\n", (dates[i].year), (dates[i].month), (dates[i].day));
    return 0;
}