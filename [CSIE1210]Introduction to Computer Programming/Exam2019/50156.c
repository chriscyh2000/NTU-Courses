#include <stdio.h>
int main(void)
{
    int a, b, v, w, x, y, z;
    scanf("%d%d%d%d%d%d%d", &a, &b, &v, &w, &x, &y, &z);
    int discount;
    int pricewithmaxrate = 1, m = 0;
    int i;
    for (i = 1; i <= b + 2; i++)
    {
        if ((i > 0) && (i < a))
        {
            discount = 0;
        }
        else if ((i >= a) && (i < b))
        {
            if (i % 2 == 1)
            {
                discount = v;
            }
            else
            {
                discount = w;
            }
        }
        else 
        {
            if (i % 3 == 0)
            {
                discount = x;
            }
            else if (i % 3 == 1)
            {
                discount = y;
            }
            else 
            {
                discount = z;
            }
        }
        if (discount * pricewithmaxrate > i * m)
        {
            pricewithmaxrate = i;
            m = discount;
        }
    }
    printf("%d\n", pricewithmaxrate);
}