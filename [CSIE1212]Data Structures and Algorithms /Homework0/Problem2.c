#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define BLACK 1
#define WHITE 0
#define UNFILLED -1

typedef struct node{
    int i;
    struct node *next;
} Node;

bool checkRows(int row, int col, 
               Node *rowContiN[row], int board[row][col]){
    for(int i = 0; i < row; i++){ 
        Node *cur = rowContiN[i];
        int j = 0, k;
        int black_need = 0, row_black = 0;

        while(cur != NULL){
            bool checkNoError = false;
            int count = 0;
            for(; j < col; j++){
                if(board[i][j] == BLACK){
                    k = 0;
                    while(board[i][j+k] == BLACK){
                        count++;
                        k++;
                        if(j + k >= col) break;
                    }
                }
                else continue; 
                if(count != cur->i) return false;
                break;
            }
            j += k + 1;
            black_need += cur->i;
            cur = cur->next;
        }
        for(j = 0; j < col; j++)
            row_black += board[i][j];
        if(row_black != black_need) return false;
    }
    return true;
}

bool checkCols(int row, int col, Node *colConti[col], int board[row][col]){
    for(int i = 0; i < col; i++){
        Node *cur = colConti[i];
        int j = 0, k;
        int black_need = 0, col_black = 0;
        while(cur != NULL){
            int count = 0;
            for(; j < row; j++){
                if(board[j][i] == BLACK){
                    k = 0;
                    while(board[j+k][i] == BLACK){
                        count++;
                        k++;
                        if(j + k >= row) break;
                    }
                }
                else continue;
                if(count != cur->i) return false;
                break;
            }
            j += cur->i + 1;
            black_need += cur->i;
            cur = cur->next;
        }
        for(j = 0; j < row; j++)
            col_black += board[j][i];
        if(col_black != black_need) return false;
    }
    return true;
}

void nextStep(int col, int cur_r, int cur_c,
              int *a, int *b){
    if(cur_c + 1 == col){
        *a = cur_r + 1;
        *b = 0;
    }
    else{
        *a = cur_r;
        *b = cur_c + 1;
    }
    return;
}

bool fillBoard(int row, int col, int cur_r, int cur_c,
               int rowSegment[], int colSegment[], 
               Node *rowConti[], Node *colConti[], 
               int board[row][col]){
    if(cur_r == row && cur_c == 0){
        if(checkRows(row, col, rowConti, board) &&
           checkCols(row, col, colConti, board))
            return true;
        else 
            return false;
    }

    int a, b;
    nextStep(col, cur_r, cur_c, &a, &b);

    board[cur_r][cur_c] = BLACK;
    if(fillBoard(row, col, a, b, rowSegment, colSegment,
                rowConti, colConti, board)) return true;

    board[cur_r][cur_c] = WHITE;
    if(fillBoard(row, col, a, b, rowSegment, colSegment,
             rowConti, colConti, board)) return true;

    return false;
}

Node *genListNode(int i, Node *next){
    Node *temp = (Node *)malloc(sizeof(Node));
    assert(temp != NULL);
    temp->i = i;
    temp->next = next;
    return temp;
}

void freeList(Node *head){
    Node *temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
    return;
}

int main(void){
    int row, col;
    scanf("%d%d", &row, &col);
    int rowSegment[row], colSegment[col];
    int board[row][col];
    Node *rowConti[row], *colConti[col];
    int i, j, temp;
    Node *node_temp;
    for(i = 0; i < row; i++){
        scanf("%d", (&rowSegment[i]));
        Node *cur;
        for(j = 0; j < rowSegment[i]; j++){
            scanf("%d", &temp);
            if(j == 0){
                rowConti[i] = genListNode(temp, NULL);
                cur = rowConti[i];
            }
            else{
                node_temp = genListNode(temp, NULL);
                cur->next = node_temp;
                cur = cur->next;
            }
        }
        if(rowSegment[i] == 0)
            rowConti[i] = genListNode(0, NULL);
        for(j = 0; j < col; j++)
            board[i][j] = UNFILLED;
    }
    for(i = 0; i < col; i++){
        scanf("%d", (&colSegment[i]));
        Node *cur;
        for(j = 0; j < colSegment[i]; j++){
            scanf("%d", &temp);
            if(j == 0){
                colConti[i] = genListNode(temp, NULL);
                cur = colConti[i];
            }
            else{
                node_temp = genListNode(temp, NULL);
                cur->next = node_temp;
                cur = cur->next;
            }
        }
        if(colSegment[i] == 0)
            colConti[i] = genListNode(0, NULL);
    }
    if(fillBoard(row, col, 0, 0, rowSegment, colSegment,
                 rowConti, colConti, board)){
        for(i = 0; i < row; i++){
            for(j = 0; j < col; j++){
                if(board[i][j] == WHITE)
                    printf("_");
                else printf("o");
            }
            printf("\n");
        }
    }
    for(i = 0; i < row; i++) freeList(rowConti[i]);
    for(i = 0; i < col; i++) freeList(colConti[i]);
    return 0;
}