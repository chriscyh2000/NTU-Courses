#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAXDIGITS 258

#define SWITCH 1
#define DONT_SWITCH 2

typedef struct BigInt{
    char digits[MAXDIGITS];
    int idx_lastDigit;
}BigInt;

void print_BigInt(BigInt *number){
    for(int i = 0; i <= number->idx_lastDigit; i++)
        printf("%d", number->digits[i]);
    printf("\n");
    return;
}

BigInt *str_to_BigInt(char *str){
    BigInt *temp = (BigInt *)malloc(sizeof(BigInt));
    assert(temp != NULL);
    temp->idx_lastDigit = strlen(str) - 1;  /*Index of an array starts from 0*/

    for(int i = 0; i <= temp->idx_lastDigit; i++)
        temp->digits[i] = str[i] - '0';

    assert(temp->idx_lastDigit <= 255);
    temp->digits[temp->idx_lastDigit + 1] = '\0';
    return temp;
}

void divided_by_2(BigInt *number){
    for(int i = 0; i <= number->idx_lastDigit; i++){
        if(number->digits[i] % 2 == 1 && i != number->idx_lastDigit)
            number->digits[i+1] += 10;
        number->digits[i] /= 2;
    }
    if(number->digits[0] == 0 && number->idx_lastDigit != 0){
        for(int i = 1; i <= number->idx_lastDigit; i++){
            number->digits[i-1] = number->digits[i];
            number->digits[i] = '\0';
        }
        number->idx_lastDigit--;
    }
    return;
}

BigInt *mutiply(BigInt *number1, BigInt *number2){
    BigInt *answer = str_to_BigInt("0");
    int i3, i3_start = 255;
    int i;
    for(i = 0; i < 256; i++)
        answer->digits[i] = 0;
    answer->digits[256] = '\0';

    int i1 = number1->idx_lastDigit, i2 = number2->idx_lastDigit;
    while(i1 != -1){
        char temp = 0;
        i3 = i3_start;
        while(i2 != -1){
            char add = number1->digits[i1] * number2->digits[i2] + temp + answer->digits[i3];
            answer->digits[i3] = add % 10;
            temp = add / 10;
            i2--;
            i3--;
        }
        answer->digits[i3] = temp;
        i3_start--;
        i1--;
        i2 = number2->idx_lastDigit;
    }
    
    i = 0;
    while(answer->digits[i] == 0) i++;
    answer->idx_lastDigit = 255 - i;
    
    if(i == 0) return answer;  /*special case*/

    for(int j = 0; j <= answer->idx_lastDigit; j++){
        answer->digits[j] = answer->digits[j+i];
        answer->digits[j+i] = '\0';
    }
    answer->digits[256-i] = '\0';
    
    return answer;
}

void minus(BigInt *number1, BigInt *number2){
    BigInt *answer = str_to_BigInt("0");
    int i3 = 255;
    int i;
    for(i = 0; i < 256; i++)
        answer->digits[i] = 0;
    answer->digits[256] = '\0';

    int i1 = number1->idx_lastDigit, i2 = number2->idx_lastDigit;
    while(i1 != -1 && i2 != -1){
        if(number1->digits[i1] < number2->digits[i2]){
            int temp = i1 - 1;
            while(temp > 0 && number1->digits[temp] == 0){
                number1->digits[temp] += 9;
                temp--;
            }
            number1->digits[temp]--;
            number1->digits[i1] += 10;
        }
        number1->digits[i1] -= number2->digits[i2];
        i1--;
        i2--;
    }

    i = 0;
    while(number1->digits[i] == 0 && i <= number1->idx_lastDigit) i++;

    /*special cases*/
    if(i == 0) return;
    if(i == number1->idx_lastDigit + 1){
        number1->idx_lastDigit = 0;
        return;
    } 

    /*normal cases*/
    for(int j = 0; j <= number1->idx_lastDigit - i; j++){
        number1->digits[j] = number1->digits[j+i];
        number1->digits[j+i] = '\0';
    }
    number1->idx_lastDigit = number1->idx_lastDigit - i;
    return;
}

int compare(BigInt *number1, BigInt *number2){
    if(number1->idx_lastDigit < number2->idx_lastDigit) return SWITCH;

    int i = 0;
    if(number1->idx_lastDigit == number2->idx_lastDigit){
        while(number1->digits[i] == number2->digits[i] && i <= number1->idx_lastDigit)
            i++;
        if(number1->digits[i] < number2->digits[i] && i <= number1->idx_lastDigit)
            return SWITCH;
    }
    return DONT_SWITCH;
}

void swap(BigInt **number1, BigInt **number2){
    BigInt *temp = *number1;
    *number1 = *number2;
    *number2 = temp;
    return;
}

int main(void){
    char strNum1[MAXDIGITS], strNum2[MAXDIGITS];
    scanf("%s %s", strNum1, strNum2);
    BigInt *m = str_to_BigInt(strNum1);
    BigInt *n = str_to_BigInt(strNum2);
    if(compare(m, n) == SWITCH) swap(&m, &n);

    BigInt *two = str_to_BigInt("2");
    BigInt *answer = str_to_BigInt("1");
    BigInt *temp;  /*used to free*/

    while(m->digits[0] > 0 && n->digits[0] >0){
        if(n->digits[n->idx_lastDigit] % 2 == 0 && m->digits[m->idx_lastDigit] % 2 == 0){
            temp = answer;
            answer = mutiply(two, answer);
            free(temp);
            divided_by_2(n);
            divided_by_2(m);
        }
        else if(n->digits[n->idx_lastDigit] % 2 == 0) divided_by_2(n);
        else if(m->digits[m->idx_lastDigit] % 2 == 0) divided_by_2(m);
        if(compare(m, n) == SWITCH) swap(&m, &n);
        minus(m, n);
    }
    free(m);
    free(two);

    print_BigInt(mutiply(answer, n));
    
    free(answer);
    free(n);
    return 0;
}