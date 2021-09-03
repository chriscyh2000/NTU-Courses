#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAXVARNUM 55
#define VARLEN 33
#define STRVALUELEN 12
bool checkvalue(char test[])
{
    if(test[0] == '0' || test[0] == '1' || test[0] == '2' || test[0] == '3' || test[0] == '4' ||\
       test[0] == '5' || test[0] == '6' || test[0] == '7' || test[0] == '8' || test[0] == '9')
        return true;
    return false;
}
int checkoperator(char test[])
{
    if(test[0] == '+') return 1;
    if(test[0] == '-') return 2;
    if(test[0] == '*') return 3;
    if(test[0] == '/') return 4;
    return (-1);
}
typedef struct node{
    int value;
    char operator;
    struct node *left;
    struct node *right;
} Node;
typedef struct variable{
    char var[VARLEN];
    Node *node;
} Variable;
Node *genTreeNode(int value, int operator, Node *left, Node *right)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    assert(temp != NULL);
    temp->value = value;
    temp->operator = operator;
    temp->left = left;
    temp->right = right;
    return temp;
}
Node *read(Variable *list, int *n)
{
    char rightbracket[2], test[VARLEN];
    char testOP[2], variable[VARLEN];
    int value, strValue;
    Node *temp, *leftnode, *rightnode;
    scanf("%s", testOP);
    scanf("%s", test);
    if(test[0] == '(') leftnode = read(list, n);
    else if(checkvalue(test))
        leftnode = genTreeNode(atoi(test), 0, NULL, NULL);
    else
    {
        leftnode = genTreeNode(0, 0, NULL, NULL);
        strcpy(list[*n].var, test);
        list[*n].node = leftnode;
        (*n)++;
    }
    scanf("%s", test);
    if(test[0] == '(') rightnode = read(list, n);
    else if(checkvalue(test))
        rightnode = genTreeNode(atoi(test), 0, NULL, NULL);
    else
    {
        rightnode = genTreeNode(0, 0, NULL, NULL);
        strcpy(list[*n].var, test);
        list[*n].node = rightnode;
        (*n)++;
    }
    scanf("%s", rightbracket);
    temp = genTreeNode(0, checkoperator(testOP), leftnode, rightnode);
    return temp;
}
int caculate(Node *node)
{
    if(node->left == NULL && node->right == NULL)
        return node->value;
    switch(node->operator)
    {
        case 1: return (caculate(node->left) + caculate(node->right));
        case 2: return (caculate(node->left) - caculate(node->right));
        case 3: return (caculate(node->left) * caculate(node->right));
        case 4: return (caculate(node->left) / caculate(node->right));
        default: return (-1);
    }
}
int main(void)
{
    Variable list[MAXVARNUM];
    char varName[VARLEN], equSign[2], varValue[STRVALUELEN], test[STRVALUELEN];
    int varN = 0;  
    Node *root;
    scanf("%s", test);
    if(!checkvalue(test))
    {
        root = read(list, &varN);
        while(scanf("%s", varName) != EOF)
        {
            scanf("%s", equSign);
            scanf("%s", varValue);
            for(int i = 0; i < varN; i++)
            {
                if(strcmp(list[i].var, varName) != 0) continue;
                list[i].node->value = atoi(varValue);
            }
        }
        printf("%d\n", caculate(root));
    }
    else printf("%d\n", atoi(test));
    return 0; 
}