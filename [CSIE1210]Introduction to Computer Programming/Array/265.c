#include <stdio.h>
int main()
{
    int X[100] = {0}, Y[100] = {0};
    int maxX, maxY, minX, minY;
    int i =1;
    int a, b, area;
    scanf("%d %d", &X[0], &Y[0]);
    minX = maxX = X[0];
    minY = maxY = Y[0];
    while (scanf("%d%d", &X[i], &Y[i]) != EOF)
    {
        if (maxX < X[i])
        {
            maxX = X[i]; 
        }
        if (maxY < Y[i])
        {
            maxY = Y[i]; 
        }
        if (minX > X[i])
        {
            minX = X[i]; 
        }
        if (minY > Y[i])
        {
            minY = Y[i]; 
        }
        i ++;
    }
    a = maxX - minX;
    b = maxY - minY;
    area = a * b;
    printf("%d\n", area);
}