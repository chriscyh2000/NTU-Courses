#include <stdio.h>
int main(){
    int n, i;
    scanf("%d", &n);
    for (i = 1 ; i <= n ; ++i)
    {
        int x1, y1, x2, y2, x3, y3, x4, y4;
        scanf("%d%d%d%d%d%d%d%d", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
        int u1, v1, u2, v2, u3, v3, u4, v4;
        int a1, b1, a2, b2, a3, b3, a4, b4;
        u1 = x1; 
        v1 = y1; 
        u4 = x4; 
        v4 = y4;
        if((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1) > 0){
            u2 = x2; 
            v2 = y2; 
            u3 = x3; 
            v3 = y3;
        }
        else{
            u2 = x3;
            v2 = y3;
            u3 = x2;
            v3 = y2;
        }
        if((u2 - u1) * (v4 - v1) - (u4 - u1) * (v2 - v1) < 0){
            a1 = u1;
            b1 = v1; 
            a2 = u4; 
            b2 = v4; 
            a3 = u2; 
            b3 = v2; 
            a4 = u3; 
            b4 = v3;
        }
        else if((u3 - u1) * (v4 - v1) - (u4 - u1) * (v3 - v1) > 0){
            a1 = u1; 
            b1 = v1; 
            a2 = u2; 
            b2 = v2; 
            a3 = u3; 
            b3 = v3; 
            a4 = u4; 
            b4 = v4;
        }
        else{
            a1 = u1;
            b1 = v1;
            a2 = u2; 
            b2 = v2;
            a3 = u4; 
            b3 = v4; 
            a4 = u3; 
            b4 = v3;
        }
 
 
        int l12, l22, l32, l42, l52, l62;
        l12 = (a1 - a2) * (a1 - a2) + (b2 - b1) * (b2 - b1); 
        l22 = (a2 - a3) * (a2 - a3) + (b2 - b3) * (b2 - b3) ;
        l32 = (a3 - a4) * (a3 - a4) + (b3 - b4) * (b3 - b4); 
        l42 = (a1 - a4) * (a1 - a4) + (b4 - b1) * (b4 - b1) ;
        l52 = (a1 - a3) * (a1 - a3) + (b3 - b1) * (b3 - b1); 
        if ((l12 == l22) && (l22 == l32) && (l32 == l42)){
            if (l12 + l22 == l52)
                printf("square\n");
            else
                printf("diamond\n");
        }
        else if ((l12 == l32) && (l22 == l42) && (l12 + l22 == l52))
            printf("rectangle\n");
        else
            printf("other\n");
    }
}