#include <stdio.h>
int main(void){
    int n, m, nbvar;
    scanf("%d%d", &n, &m);
    nbvar = n * m;
    int nbclause;
    int i, j, k;
    int a = 1, b = 1;
    for(i = 1; i <= n; ++i)
        a = a * i;
    for(i = 1; i <= n - 2; ++i)
        b = b * i;
    nbclause = (a / (b * 2)) * m + n;
    int var[n][m], index = 1;
    for(i = 0; i < n; ++i){
        for(j = 0; j < m; ++j){
            var[i][j] = index;
            ++index;
        }
    }
    FILE *fp;
    fp = fopen("output.cnf", "w");
    fprintf(fp, "c\n");
    fprintf(fp, "c start with comments\n");
    fprintf(fp, "c\n");
    fprintf(fp, "c\n");
    fprintf(fp, "c the pigeonhole problem with %u pigeons and %u holes\n", n, m);
    fprintf(fp, "p cnf %d %d\n", nbvar, nbclause);
    for(i = 0; i < n; ++i){
        for(j = 0; j < m; ++j)
            fprintf(fp, "%d ",var[i][j]);
        fprintf(fp, "0\n");
    }
    for(j = 0; j < m; ++j){
        for(i = 0; i < n - 1; ++i){
            for(k = i + 1; k < n; ++k){
                fprintf(fp, "-%d -%d 0", var[i][j], var[k][j]);
                fprintf(fp, "\n");
            }
        }    
    }
    fclose(fp);
    return 0; 
}