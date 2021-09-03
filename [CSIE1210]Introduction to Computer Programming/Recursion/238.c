#include <stdio.h>

int subsetSum(int desiredSum, int elementsNum, int index, 
              int elements[elementsNum], int foundSet)
{
    if(index == elementsNum)
        return foundSet;
    if(desiredSum - elements[index] > 0)
        return (foundSet + subsetSum(desiredSum - elements[index], 
                                     elementsNum - 1, 0,
                                     &(elements[index + 1]), 0)
                         + subsetSum(desiredSum, elementsNum - 1, 0,
                                     &(elements[index + 1]), 0));
    else if(desiredSum == elements[index])
        return (foundSet + subsetSum(desiredSum, elementsNum - 1, 0,
                                     &(elements[index + 1]), 0) + 1);
    else
        return (subsetSum(desiredSum, elementsNum - 1, 0,
                &(elements[index + 1]), 0));  
}
int main(void)
{
    int i, k, test;
    scanf("%d", &k);
    int elements[k];
    for(i = 0; i < k; i++)
        scanf("%d", &elements[i]);
    while(scanf("%d", &test) != EOF)
        printf("%d\n", subsetSum(test, i, 0, elements, 0));
    return 0;
}