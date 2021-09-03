#include <stdio.h>
void bubbleSort(int array[], int n)
{
    int temp;
    for(int i = n - 2; i >= 0; i--)
    {
        for(int j = 0; j <= i; j++)
        {
            if(array[j] > array[j + 1])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    return;
}
void cpyArray(int array[], int temp[], int n)
{
    for(int i = 0; i < n; i++)
        temp[i] = array[i];
    return;
}
void listPermutation(int number[], int n, int index)
{
    int temp[n];
    int t;
    bubbleSort(&number[index], n - index);
    cpyArray(number, temp, n);
    if(index == n)
    {
        for(int i = 0; i < n; i++)
        {
            if(i != n - 1) printf("%d ", number[i]);
            else printf("%d\n", number[i]);
        }
        return;
    }
    for(int i = index; i < n; i++)
    {
        t = number[index];
        number[index] = number[i];
        number[i] = t;
        listPermutation(number, n, index + 1);
        cpyArray(temp, number, n);
    }
}
int main(void)
{
    int n;
    scanf("%d", &n);
    int number[n];
    for(int i = 0; i < n; i++)
        scanf("%d", &number[i]);
    listPermutation(number, n, 0);
    return 0;
}