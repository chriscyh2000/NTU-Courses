#include <stdio.h>
int main(){
    int a, b, c, d, e, f, g, h, i, j;
    int p, q;
    scanf("%d%d%d%d%d%d%d", &a, &b, &c, &d, &e, &f, &g);
    if (a < 0)
        b = -b ;
    if (e < 0)
        f = -f ;
    switch (d){
    case 0 :{
        int u;
        int k1, k2, k3, gcd;
        u = g * b + f * c;
        k1 = g * c ; k2 = u % k1; 
        q = g * c ; p = u % q;
        if (k2 != 0){
            while (k1 % k2 != 0){
                k3 = k1;
                k1 = k2;
                k2 = k3 % k2;
            }
            gcd = k2;
            h = (a + e) + u / (g * c);
            i = p / gcd; 
            j =  q / gcd;
        }
        else{
            h = (a + e) + u / (g * c);
            i = p; 
            j = 1; 
        }
        break;
    }
    case 1 :{
        int u;
        int k1, k2, k3, gcd;
        u = g * b - f * c;
        k1 = g * c; 
        k2 = u % k1;
        q = g * c; 
        p = u % q;
        if (k2 != 0){   
            while(k1 % k2 != 0){
                k3=k1;
                k1=k2;
                k2=k3%k2;
            }
            gcd = k2;
            h = (a - e) + u / (g * c);
            i = p / gcd; 
            j =  q / gcd;
        }    
        else{
            h = (a - e) + u / (g * c);
            i = p; 
            j = 1;
        }
        break;
    }
    case 2 :
    {   
        int u;
        int k1, k2, k3, gcd;
        u = (a * f * c) + (b * g * e) + (b * f);
        k1 = g * c; 
        k2 = u % k1;
        q = g * c; 
        p = u % q;
        if(k2 != 0){
            while(k1 % k2 != 0){
                k3=k1;
                k1=k2;
                k2=k3%k2;
            }
            gcd = k2;
            h = (a * e) + u / (g * c);
            i = p / gcd;
            j =  q / gcd;
        }
        else{
            h = (a * e) + u / (g * c);
            i = p;
            j = 1;
        }
        break;
    }
    case 3 :{
        int u ;
        int k1, k2, k3, gcd;
        u = (c * a + b) * g;
        k1 = (e * g + f) * c; 
        k2 = u % k1;
        q = ( e * g + f ) * c; 
        p = u % q;
        if (k2 != 0){
            while(k1%k2!=0){
                k3=k1;
                k1=k2;
                k2=k3%k2;
            }
            gcd = k2;
            h = u / q ;
            i = p / gcd;
            j = q / gcd ;
        }
        else{
            h = u / q ;
            i = p; 
            j = 1;
        }
        break;
    }
    default :
        break;
    }
    if(i < 0) i = -i;
    if (j < 0) j = -j;
    printf("%d\n%d\n%d\n", h, i, j);
}