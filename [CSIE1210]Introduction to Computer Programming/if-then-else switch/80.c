#include <stdio.h>
int main()
{
    int x1, y1, x2, y2, x3, y3, n, i, a, b, c;
    scanf("%d", &n);
    for(i = 1; i <= n; ++i){
        scanf("%d%d%d%d%d%d", &x1, &y1, &x2, &y2, &x3, &y3);
        a = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); 
        b = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3);
        c = (x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3);
 
        if(((a > b) && (b > c)) || ((a > c) && (c > b))){
            if (a > b + c)
                printf("obtuse\n");
            if (a == b + c)
                printf("right\n");
            if (a < b + c)
                printf("acute\n");
        }
        else if(((b > a) && (a > c)) || ((b > c) && (c > a))){
            if (b > a + c)
                printf("obtuse\n");
            if (b == a + c)
                printf("right\n");
            if (b < a + c)
                printf("acute\n");
        }
        else if(((c > a) && (a > b)) || ((c > b) && (b > a))){
            if ( c > b + a )
                printf("obtuse\n");
            if ( c == b + a )
                printf("right\n");
            if ( c < b + a )
                printf("acute\n");
        }
        else
            printf("isosceles\n");
    }    
}