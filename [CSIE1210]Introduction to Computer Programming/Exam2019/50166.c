#include <stdio.h>
int main(void){
    int d, k;
    scanf("%d", &d);
    double coefficient[d+1];
    for(int i = d; i >= 0; --i)
        scanf("%lf", &coefficient[i]);
    double x1, temp, f, f_prime;
    scanf("%d", &k);
    scanf("%lf", &x1);
    for(int i = 0; i < k; ++i){
        printf("%.4lf ", x1);
        temp = 1;
        f = 0;
        f_prime = 0;
        for(int n = 0; n < d + 1; ++n){
            f += temp * coefficient[n];
            temp *= x1;
        }
        temp = 1;
        printf("%.4f\n", f);
        for(int n = 1; n < d + 1; ++n){
            f_prime += temp * coefficient[n] * n;
            temp *= x1;
        }
        x1 = x1 - f / f_prime;
    }
}