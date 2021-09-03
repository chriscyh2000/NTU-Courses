#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define VAR 150
#define NAMELENGTH 150
#define LINE 1200
enum Operator
    {ADD, SUB, MUL, DIV, MOD, EQL, NOTEQL, SMALLER, LARGER, SMALLEREQL, LARGEREQL};
enum StatementType
    {STOP, IF, GOTO, PRINT, ASSIGN};
/* find */
int find(char *string, char  *table[])
{
    int index = 0;
    while(table[index] != NULL)
    {
        if (strcmp(string, table[index]) == 0)
            return index;
        index++;
    }
    return (-1);  /* NOT FOUND!! */
}
/* findOprCode */
int findOprCode(char *sign)
{
    char *OperatorTable[] = {"+", "-", "*", "/", "%", "==", "!=", "<", ">", "<=", ">=", NULL};
    int i = find(sign, OperatorTable);
    if(i == -1)
        exit(-1);
    return i;
}
int findVariable(char *var, char *varNameTable[])
{
    int index = find(var, varNameTable);
    if(index == -1)
        exit(-1);
    return index;
}
/* processIF */
void processIF(int lineNum, char *varNameTable[], int code[], int opd1[], int opd2[], 
               int compopr[], int gotoLine[])
{
    char gotoString[NAMELENGTH];
    char opd[NAMELENGTH], opr[NAMELENGTH];
    code[lineNum] = IF;
    scanf("%s", opd);
    opd1[lineNum] = findVariable(opd, varNameTable);
    scanf("%s", opr);
    compopr[lineNum] = findOprCode(opr);
    scanf("%s", opd);
    opd2[lineNum] = findVariable(opd, varNameTable);
    scanf("%s", gotoString);
    assert(strcmp(gotoString, "GOTO") == 0);
    scanf("%d", &(gotoLine[lineNum]));
}
/* processASSIGN */
void processASSIGN(int lineNum, char *first, char *varNameTable[], int code[], 
                   int opd1[], int opd2[], int compopr[], int target[]) /* target是要assign的對象的index */
{
    char opd[NAMELENGTH], opr[NAMELENGTH];
    code[lineNum] = ASSIGN;
    target[lineNum] = findVariable(first, varNameTable);
    char assignString[NAMELENGTH];
    scanf("%s", assignString);
    assert(strcmp(assignString, "=") == 0);
    scanf("%s", opd);
    opd1[lineNum] = findVariable(opd, varNameTable);
    scanf("%s", opr);
    compopr[lineNum] = findOprCode(opr);
    scanf("%s", opd);
    opd2[lineNum] = findVariable(opd, varNameTable);
}
/* processCode */
void processCode(char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], 
                 int gotoLine[], int target[])
{
    char first[NAMELENGTH];
    char opd[NAMELENGTH];
    int lineNum = 1;
    char *firstTable[] = {"STOP", "IF", "GOTO", "PRINT", NULL};
    while(scanf("%s", first) != EOF)
    {
        int firstType = find(first, firstTable);
        switch(firstType)
        {
            case STOP:
                code[lineNum] = STOP;
                break;
            case IF:
                code[lineNum] = IF;
                processIF(lineNum, varNameTable, code, opd1, opd2, 
                          compopr, gotoLine);
                break;
            case GOTO:
                code[lineNum] = GOTO;
                scanf("%d", &(gotoLine[lineNum]));
                break;
            case PRINT:
                code[lineNum] = PRINT;
                scanf("%s", opd);
                target[lineNum] = findVariable(opd, varNameTable);
                break;
            default:
                code[lineNum] = ASSIGN;
                processASSIGN(lineNum, first, varNameTable, code, opd1, opd2, compopr, target);
                break;
        }
        lineNum++;
    }
}
/* compare */
int compare(int compare1, int compare2, int operator)
{
    switch(operator)
    {
        case EQL:
            return(compare1 == compare2);
        case NOTEQL:
            return(compare1 != compare2);
        case SMALLER:
            return(compare1 < compare2);
        case LARGER:
            return(compare1 > compare2);
        case SMALLEREQL:
            return(compare1 <= compare2);
        case LARGEREQL:
            return(compare1 >= compare2);
        default:
            printf("invalid comp %d\n", operator);
            exit(-1);
    }
}
/* arithmetic */
int arithmetic(int operand1, int operand2, int operator)
{
    switch(operator)
    {
        case ADD:
            return(operand1 + operand2);
        case SUB:
            return(operand1 - operand2);
        case MUL:
            return(operand1 * operand2);
        case DIV:
            return(operand1 / operand2);
        case MOD:
            return(operand1 % operand2);
        default:
            printf("invalid comp %d", operator);
            exit(-1);
    }
}
/* runcode */
void runCode(char varName[][NAMELENGTH], int varValue[], int code[], int opd1[], int opd2[], 
            int compopr[], int gotoLine[], int target[])
{
    int line = 1;
    while(code[line] != STOP)
    {
        switch(code[line])
        {
            case IF:
                if(compare(varValue[opd1[line]], varValue[opd2[line]], compopr[line]))
                    line = gotoLine[line];
                else
                    line++;
                break;
            case GOTO:
                line = gotoLine[line];
                break;
            case ASSIGN:
                varValue[target[line]] = arithmetic(varValue[opd1[line]],
                                                    varValue[opd2[line]], compopr[line]);
                line++;
                break;
            case PRINT:
                printf("%d\n", varValue[target[line]]);
                line++;
                break;
            default:
                printf("invalid code %d\n", code[line]);
                exit(-1);
        }
    }
}
/* readVariable */
void readVariable(char varName[][NAMELENGTH], int varValue[], char *varNameTable[])
{
    int count = 0;
    char name[NAMELENGTH];
    scanf("%s", name);
    while(strcmp(name, "END") != 0)
    {
        char assignString[NAMELENGTH];
        strcpy(varName[count], name);
        scanf("%s", assignString);
        assert(strcmp(assignString, "=") == 0);
        scanf("%d", &(varValue[count]));
        varNameTable[count] = varName[count];
        count++;
        scanf("%s", name);
    }
    varNameTable[count] = NULL;
}
/* main */
int main(void)
{
    char varName[VAR][NAMELENGTH], *varNameTable[VAR];
    int varValue[VAR];
    readVariable(varName, varValue, varNameTable);
    int code[LINE];
    int opd1[LINE], opd2[LINE], compopr[LINE]; 
    int gotoLine[LINE];
    int target[LINE];
    processCode(varNameTable, code, opd1, opd2, compopr, gotoLine, target);
    runCode(varName, varValue, code, opd1, opd2, compopr, gotoLine, target);
    return 0;
}