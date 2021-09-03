#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
typedef struct node{
    int data;
    struct node *left;
    struct node *right;
} Node;
void printTree(Node *tree, int type) /* HLHR: type = 0, HRHL: type = 1 */
{
    if(tree == NULL) return;
    assert(type == 0 || type == 1);
    if(type == 0)
    {
        printf("%d\n", tree->data);
        printTree(tree->left, 1);
        printf("%d\n", tree->data);
        printTree(tree->right, 1);
    }
    else if(type == 1)
    {
        printf("%d\n", tree->data);
        printTree(tree->right, 0);
        printf("%d\n", tree->data);
        printTree(tree->left, 0);
    }
    return;
}
Node *buildTree(char *inputStr)
{
    if(inputStr[0] != '(')
    {
        Node *temp = (Node *)malloc(sizeof(Node));
        assert(temp != NULL);
        temp->data = atoi(inputStr);
        temp->left = NULL;
        temp->right = NULL;
        return temp;
    }
    /* 把前後兩個括號拿掉之後，一部分形成左樹另一部分形成右樹，
    左樹和右樹的總左括號＝總右括號，中間以一個逗號區隔*/
    int length = strlen(inputStr);
    int leftbracket = 0, rightbracket = 0, mid;
    inputStr[length - 1] = '\0';
    inputStr++; length -= 2;
    for(int i = 0; i < length; i++)
    {
        if(inputStr[i] == '(') leftbracket++;
        if(inputStr[i] == ')') rightbracket++;
        if(inputStr[i] == ',' && leftbracket == rightbracket)
        {
            mid = i;
            break;
        }
    }
    /* build up the tree of each side, 
    and save the data into the rootNode */
    Node *temp = (Node *)malloc(sizeof(Node));
    assert(temp != NULL);
    temp->right = buildTree(inputStr + mid + 1);
    inputStr[mid] = '\0';
    temp->left = buildTree(inputStr);
    temp->data = temp->left->data + temp->right->data;
    return temp; 
}
int main(void)
{
    char inputStr[4001];
    scanf("%s", inputStr);
    Node *root = buildTree(inputStr);
    printTree(root, 0);
    return 0;
}