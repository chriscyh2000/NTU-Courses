#include <stdio.h>
int main()
{
    int a, b, c, d;
    int e, f, g;
    scanf("%d%d%d%d", &a, &b, &c, &d);
    scanf("%d%d%d", &e, &f, &g);
    int sunkvolumn, additionaldepth, newdepth;
    if (d > g)
    {
        sunkvolumn = e * f * g;
        additionaldepth = sunkvolumn / (a * b);
        newdepth = d + additionaldepth;
    }
    else if (d == 0)
    {
        newdepth = 0;
    }
    else
    {
        sunkvolumn = e * f * d;
        if ((a * b - e * f) != 0)
        {
            additionaldepth = sunkvolumn / (a * b - e * f);
            newdepth = d + additionaldepth;
            if (newdepth > g)
            {
                int height;
                height = (sunkvolumn - (a * b - e * f) * (g - d)) / (a * b);
                newdepth = g + height;
            }
        }
        else 
        {
            if (g < c)
            {
                newdepth = d + g;
            }
            else
            {
                newdepth = 0;
            }
        }
    }
    if (newdepth < c)
    {
        printf("%d\n", newdepth);
    }
    else 
    {
        printf("%d\n", c);
    }
}
