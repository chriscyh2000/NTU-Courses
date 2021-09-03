#include <stdio.h>
int pickUpNum(int limit, int N, int max,
              int numbers[N], int index)
{
    if(N - index < limit || max < 0) return 0;
    if((limit <= 0) && (index == N)) return 1; 
    if(numbers[index] > max)
        return pickUpNum(limit, N, max, numbers, index + 1);
    return (pickUpNum(limit - 1, N, max - numbers[index], numbers, index + 1) +\
            pickUpNum(limit, N, max, numbers, index + 1));
}
int main(void)
{
    int limit, N, max;
    scanf("%d%d%d", &limit, &N, &max);
    int numbers[N];
    for(int i = 0; i < N; i++)
        scanf("%d", &numbers[i]);
    printf("%d\n", pickUpNum(limit, N, max, numbers, 0));
}