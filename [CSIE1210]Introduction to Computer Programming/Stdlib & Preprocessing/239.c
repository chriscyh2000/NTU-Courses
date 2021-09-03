#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXDATA 10000
typedef struct expenseData{
    int year, month, day;
    char category[40];
    int expense;
} ExpenseData;
ExpenseData list[MAXDATA];
int compare(const void *data1, const void *data2)
{
    ExpenseData expense1 = *(ExpenseData *)data1;
    ExpenseData expense2 = *(ExpenseData *)data2;
    if(expense1.year != expense2.year) 
        return (expense1.year - expense2.year);
    if(expense1.month != expense2.month) 
        return (expense1.month - expense2.month);
    if(expense1.day != expense2.day) 
        return (expense1.day - expense2.day);
    return strcmp(expense1.category, expense2.category);
}
int main(void)
{
    int i = 0, n;
    while(scanf("%d%d%d%s%d", &(list[i].year), &(list[i].month), &(list[i].day), 
                              list[i].category, &(list[i].expense)) != EOF)
        i++;   
    qsort(list, i, sizeof(ExpenseData), compare);
    for(n = 0; n < i; n++)
    {
        if(list[n].year == list[n+1].year &&
           list[n].month == list[n+1].month &&
           list[n].day == list[n+1].day &&
           strcmp(list[n].category, list[n+1].category) == 0)
        {
            list[n+1].expense += list[n].expense;
            list[n].year = -1;
        }
    }
    for(n = 0; n < i; n++)
    {
        if(list[n].year == -1) continue;
        printf("%d %d %d %s %d\n", list[n].year, list[n].month, list[n].day,
                                   list[n].category, list[n].expense);
    }
    return 0;
}