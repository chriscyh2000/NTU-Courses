#include <stdio.h>
#include <stdbool.h>
bool better(int a, int b)
{
    return ((a > b)? true : false);
}
int greatestValue(int n, int limit, 
                  int weight[], int value[], 
                  int index, int foundValue)
{
    if(index == n)
        return foundValue;
    int a, b;
        a = greatestValue(n - 1, limit - weight[index],
                          &(weight[index + 1]), &(value[index + 1]), 
                          0, foundValue + value[index]);
        b = greatestValue(n - 1, limit,
                          &(weight[index + 1]), &(value[index + 1]), 
                          0, foundValue);
    if(limit - weight[index] >= 0) 
        if(better(a, b)) return a;
    return b;
}
int main(void)
{
    int n, limit;
    scanf("%d%d", &n, &limit);
    int weight[n], value[n];
    int index = 0, foundValue = 0;
    for(int i = 0; i < n; i++)
        scanf("%d%d", &weight[i], &value[i]);
    printf("%d\n", greatestValue(n, limit, weight, value, index, foundValue));
}