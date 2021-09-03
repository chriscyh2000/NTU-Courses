#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct node{
    int key;
    int level;
    struct node *left;
    struct node *right;
} Node;
Node *genTreeNode(int key, int level, Node *left, Node *right)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->key = key;
    temp->level = level;
    temp->left = left;
    temp->right = right;
    return temp;
}
void insertTree(int inkey, int inlevel, Node **root)
{
    if(*root == NULL)
    {
        *root = genTreeNode(inkey, inlevel, NULL, NULL);
        return;
    }
    Node *current, *previous;
    current = *root; previous = NULL;
    while(current != NULL)
    {
        previous = current;
        if(inkey < current->key) current = current->left;
        else current = current->right;
    }
    Node *temp = genTreeNode(inkey, inlevel, NULL, NULL);
    if(previous->key > inkey) previous->left = temp;
    else previous->right = temp;
    return;
}
Node *commonRoot(int key1, int key2, Node *root)
{
    Node *current = root;
    while((key1 < current->key && key2 < current->key) || (key1 > current->key && key2 > current->key))
    {
        if(key1 < current->key && key2 < current->key) current = current->left;
        else current = current->right;
    }
    return current;
}
int searchLevel(int inkey, Node *root){
	Node *current = root;
	while(current != NULL){
		if(current->key == inkey) return current->level;
		else if(inkey > current->key) current = current->right;
		else current = current->left;
	}
	return -1;
}
int distance(int key1, int key2, Node *root)
{
    Node *common = commonRoot(key1, key2, root);
    return (searchLevel(key1, root) + searchLevel(key2, root) - 2 * common->level);
}
int main(void)
{
    int n, p;
    scanf("%d", &n);
    int key[n], level[n], key1, key2;
    for(int i = 0; i < n; i++)
        scanf("%d%d", &key[i], &level[i]);
    for(int i = n - 2; i >= 0; i--)
    {
        for(int j = 0; j <= i; j++)
        {
            if(level[j] > level[j+1])
            {
                int tempkey, templevel;
                tempkey = key[j];
                key[j] = key[j+1];
                key[j+1] = tempkey;
                templevel = level[j];
                level[j] = level[j+1];
                level[j+1] = templevel;
            }
        }
    }
    Node *root = NULL;
    for(int i = 0; i < n; i++) insertTree(key[i], level[i], &root);
    scanf("%d", &p);
    for(int i = 0; i < p; i++)
    {
        scanf("%d%d", &key1, &key2);
        printf("%d\n", distance(key1, key2, root));
    }
    return 0;
}