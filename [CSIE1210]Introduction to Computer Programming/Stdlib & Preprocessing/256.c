#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAXSIZE 1000
int checkTime(long int i)
{
    int time = 0;
    while(i > 1)
    {
        i /= 2;
        time++;
    }
    return time;
}
int compare(const void *data1, const void *data2)
{
    long int *i1 = (long int *)data1;
    long int *i2 = (long int *)data2;
    int a = 0, b = 0;
    int checki1 = checkTime(*i1);
    int checki2 = checkTime(*i2);
    int n = (checki1 > checki2) ? checki1 : checki2;
    for(int i = 0; i <= n; i++)
    {
        if(*i1 & ((long int) 1 << i)) a++;
        if(*i2 & ((long int) 1 << i)) b++;
        if(i >= checki1 && a < b) return -1;
        if(i >= checki2 && a > b) return 1;
    }
    if(a < b) return -1;
    if(a > b) return 1;
    if(a == b)
    {
        if(*i1 < *i2) return -1;
        if(*i1 > *i2) return 1;
    }
    return 0;
} 
int main(void)
{
    long int array[MAXSIZE];
    int i = 0;
    while(scanf("%ld", &array[i]) != EOF) i++;
    qsort(array, i, sizeof(long int), compare);
    for(int j = 0; j < i; j++)
        printf("%ld\n", array[j]);
    return 0;
}