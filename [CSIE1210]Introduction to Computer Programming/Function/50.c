#include <stdio.h>
long int max(int a, int b);
long int min(int a, int b);
int main(void)
{
    long int Arx, Ary, Alx, Aly;//A長方形的右上角座標與左上角座標
    long int Brx, Bry, Blx, Bly;//B長方形的右上角座標與左上角座標
    long int Crx, Cry, Clx, Cly;//C長方形的右上角座標與左上角座標
    scanf("%ld%ld%ld%ld", &Alx, &Aly, &Arx, &Ary);
    scanf("%ld%ld%ld%ld", &Blx, &Bly, &Brx, &Bry);
    scanf("%ld%ld%ld%ld", &Clx, &Cly, &Crx, &Cry);
    long int A, B, C, AB, AC, BC, ABC;
    long int unionarea;
    A = (Arx - Alx) * (Ary - Aly);  B= (Brx - Blx) * (Bry - Bly); C = (Crx - Clx) * (Cry - Cly);

    AB = (min(Arx, Brx) - max(Alx, Blx)) * (min(Ary, Bry) - max(Aly, Bly));
    if ((min(Arx, Brx) - max(Alx, Blx) < 0) || (min(Ary, Bry) - max(Aly, Bly) < 0))
        AB = 0;

    AC = (min(Arx, Crx) - max(Alx, Clx)) * (min(Ary, Cry) - max(Aly, Cly));
    if ((min(Arx, Crx) - max(Alx, Clx) < 0) || (min(Ary, Cry) - max(Aly, Cly) < 0))
        AC = 0;

    BC = (min(Brx, Crx) - max(Blx, Clx)) * (min(Bry, Cry) - max(Bly, Cly));
    if ((min(Brx, Crx) - max(Blx, Clx) < 0) || (min(Bry, Cry) - max(Bly, Cly) < 0))
        BC = 0;

    ABC = (min(min(Arx, Brx), Crx) - max(max(Alx, Blx), Clx)) * (min(min(Ary, Bry), Cry) - max(max(Aly, Bly), Cly));
    if ((min(min(Arx, Brx), Crx) - max(max(Alx, Blx), Clx) < 0) || (min(min(Ary, Bry), Cry) - max(max(Aly, Bly), Cly) < 0))
        ABC = 0;

    unionarea = A + B + C - AB - AC - BC + ABC;
    printf("%ld\n", unionarea);
    return 0;
}
long int max(int a, int b)
{
    long int answer = b;
    if (a > b) answer = a;
    return answer;
}
long int min(int a, int b)
{
    long int answer = b;
    if (a < b) answer = a;
    return answer;
}