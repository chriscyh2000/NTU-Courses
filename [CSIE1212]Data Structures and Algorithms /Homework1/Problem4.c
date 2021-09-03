#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define STACKSIZE 1000050
#define STRLEN 1000050

/*set up the function for stack*/
typedef struct stack{
    double cal_elements[STACKSIZE];
    int top;
}cal_Stack;

void initStack(cal_Stack *st){
    st->top = 0;
    return;
}
bool stack_full(cal_Stack *st){
    return (st->top >= STACKSIZE);
}
bool stack_empty(cal_Stack *st){
    return (st->top <= 0);
}
void push(double data, cal_Stack *st){
    if(stack_full(st))
        exit (-1);
    st->cal_elements[st->top++] = data;
    return;
}
double pop(cal_Stack *st){
    if(stack_empty(st))
        exit (-1);
    return (st->cal_elements[--st->top]);
}

/*set up the functiom for calculation*/
int checkPiority(char operator){
    switch(operator){
        case '(':   
            return 0;
        case '+': case '-':
            return 1;
        default:
            return 2;
    }
}
bool checkDigit(char c){
    if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
       c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
       return true;
    return false;
}
void infix_to_postfix(char infix[], int postfix[], char opStack[]){
    int i = 0, j = 0, top = 0;
    while(infix[i] != '\0'){
        switch(infix[i]){
            case '(':
                opStack[top++] = infix[i];
                break;
            case '+': case '-': case '*': case '/':
                while(top > 0 && checkPiority(opStack[top-1]) >= checkPiority(infix[i])){
                    if(opStack[top-1] == '+')
                        postfix[j++] = -1;
                    else if(opStack[top-1] == '-')
                        postfix[j++] = -2;
                    else if(opStack[top-1] == '*')
                        postfix[j++] = -3;
                    else if(opStack[top-1] == '/')
                        postfix[j++] = -4; /*once we read the postfix and find a negtive number,
                                             we know it is an operator.*/
                    top--;
                }
                opStack[top++] = infix[i];
                break;
            case ')':
                while(opStack[top-1] != '('){
                    if(opStack[top-1] == '+')
                        postfix[j++] = -1;
                    else if(opStack[top-1] == '-')
                        postfix[j++] = -2;
                    else if(opStack[top-1] == '*')
                        postfix[j++] = -3;
                    else if(opStack[top-1] == '/')
                        postfix[j++] = -4;
                    top--;
                }
                top--;
                break;
            default:
                postfix[j++] = atoi(&infix[i]);
                while(checkDigit(infix[i])) i++;
                continue;
        }
        i++;
    }
    while(top > 0){
        if(opStack[top-1] == '+')
            postfix[j++] = -1;
        else if(opStack[top-1] == '-')
            postfix[j++] = -2;
        else if(opStack[top-1] == '*')
            postfix[j++] = -3;
        else if(opStack[top-1] == '/')
            postfix[j++] = -4;
        top--;
    }
    postfix[j++] = 0;
    return;
}
double calculateAnswer(int postfix[], cal_Stack *cal_stack){
    int i = 0;
    initStack(cal_stack);
    while(postfix[i] != 0){
        if(postfix[i] > 0) push(postfix[i], cal_stack);
        else{
            double temp;
            if(postfix[i] == -1){
                temp = pop(cal_stack) + pop(cal_stack);
                push(temp, cal_stack);
            }
            else if(postfix[i] == -2){
                double temp2 = pop(cal_stack);
                temp = pop(cal_stack) - temp2;
                push(temp, cal_stack);
            }
            else if(postfix[i] == -3){
                temp = pop(cal_stack) * pop(cal_stack);
                push(temp, cal_stack);
            }
            else if(postfix[i] == -4){
                double temp2 = pop(cal_stack);
                temp = pop(cal_stack) / temp2;
                push(temp, cal_stack);
            }
        }
        i++;
    }
    return cal_stack->cal_elements[0];
}
int main(void){
    char infix[STRLEN] = {'\0'};
    int postfix[STRLEN] = {0};
    char opStack[STACKSIZE] = {'\0'};
    cal_Stack *cal_stack = (cal_Stack *)malloc(sizeof(cal_Stack));
    assert(cal_stack != NULL);
    while(scanf("%s", infix) != EOF){
        infix_to_postfix(infix, postfix, opStack);
        printf("%.14f\n", calculateAnswer(postfix, cal_stack));
    }
    return 0;
}