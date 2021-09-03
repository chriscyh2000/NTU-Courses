#include <stdio.h>
int main(){
    int a, b, c;
    int d, e, f;
    scanf("%d%d%d", &a, &b, &c);
    int g, h;
    if (c < 0) g = -c;
    else g = c;

    if(b < 0) h = -b;
    else h = b;

    if (c != 0){
        int factor[g+1];
        factor[0] = 0;
        int i;
        for (i = 1; i <= g ; ++i){
            if(g % i == 0)
                factor[i] = 1 ;
            else 
                factor[i] = 0 ;
        }
        int n=0 ;
        for(i = 1; i <= g; ++i){
            if (factor[i] == 1) ++n;
        }
        n = 2 * n;

        int number[n+1]; 
        number[0] = 0 ;
        int j, k = g + 1;
        for(j = 1; j <= n / 2 ; ++j){
            for(i = k - 1; i >= 1; --i){
                if(factor[i] == 1){
                    number[j] = -i;
                    k = i;
                    break;
                }
            }
        }
        k = 0;
        for(j = n/2 + 1; j <= n; j++){
            for(i = k + 1; i <= g; ++i){
                if(factor[i] == 1){
                    number[j] = i;
                    k = i;
                    break;
                }
            }
        }
        int p, q, r;
        for(i = 1; i <= n; ++i){
            for(j = i; j <= n; ++j){
                for (k = j; k <= n; ++k){
                    p = number[i];
                    q = number[j];
                    r = number[k];
                    if((a == p + q + r) && (b == p * q + q * r + p * r) && (c == p * q * r)){
                        d = p; 
                        e = q; 
                        f = r;
                    }
                }
            }
        }
    }
    else{
        if(b != 0){
            int factor[h+1];
            factor[0] = 0;
            int i;
            for(i = 1; i <= h; ++i){
                if(h % i == 0)
                    factor[i] = 1;
                else 
                    factor[i] = 0;
            }
            int n = 0;
            for(i = 1; i <= h; ++i){
                if(factor[i] ==1)
                    n++;
            }
            n = 2 * n;
            int number[n+1];
            number[0] = 0;
            int j, k = h + 1;
            for(j = 1; j <= n / 2; ++j){
                for(i = k - 1; i >= 1; --i){
                    if(factor[i] == 1){
                        number[j] = -i ;
                        k = i ;
                        break;
                    }
                }
            }
            k = 0 ;
            for(j = n / 2 + 1; j <= n; ++j){
                for(i = k + 1; i <= h; ++i){
                    if(factor[i] == 1){
                        number[j] = i;
                        k = i;
                        break;
                    }
                }
            }
            int p, q;
            for (i = 1; i <= n; ++i){
                for (j = i; j <= n; ++j){
                    p = number[i];
                    q = number[j]; 
                    if((a == p + q) && (b == p * q)){
                        d = p; 
                        e = q; 
                        f = 0; 
                    }
                }
            }
            int temp;
            if(e > f){
                temp = e; 
                e = f; 
                f = temp; 
            }
            if(d > e){
                temp = d; 
                d = e; 
                e = temp;
            }
            if(e > f){
                temp = e; 
                e = f; 
                f = temp;
            }
        }
        else{
            d = a; 
            e = 0; 
            f = 0;
            int temp;
            if(e > f){
                temp = e; 
                e = f; 
                f = temp; 
            }
            if(d > e){
                temp = d; 
                d = e; 
                e = temp;
            }
            if(e > f){
                temp = e; 
                e = f; 
                f = temp;
            }
        }
    }
    printf("%d %d %d\n",d, e, f);
}