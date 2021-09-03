#include "node.h"
#include <stdlib.h>
#include <stdio.h>
void my_print(struct node *node, int path[], int level)
{
    if(node == NULL) return;
    path[level] = node->data;
    if(node->left == NULL && node->right == NULL)
    {
        for(int i = 0; i < level; i++)
            printf("%d ", path[i]);
        printf("%d\n", path[level]);
        return;
    }
    my_print(node->left, path, level + 1);
    my_print(node->right, path, level + 1);
    return;
}
void print_all_paths(struct node *root)
{
    int path[1001] = {0};
    my_print(root, path, 0);
    return 0;
}