#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct node{
    short cobinLabel;
    struct node *XORpointer;
}XORnode;

typedef struct rail{
    struct node *head;
    struct node *tail;
}Rail;


XORnode *genCobinNode(short cobinLabel, XORnode *XORpointer){
    XORnode *temp1;
    temp1 = (XORnode *)malloc(sizeof(XORnode));
    assert(temp1 != NULL);
    temp1->cobinLabel = cobinLabel;
    temp1->XORpointer = XORpointer;
    return temp1;
}
XORnode *XOR(XORnode *a, XORnode *b){
    XORnode *temp1;
    temp1 = (XORnode *)((intptr_t)(a) ^ (intptr_t)(b));
    return temp1;
}
void enter(Rail *rail, short cobin_label){
    XORnode *temp1, *temp2;
    temp1 = genCobinNode(cobin_label, XOR(rail->tail, NULL));
    if(rail->tail == NULL){
        rail->head = temp1;
        rail->tail = temp1;
        return;
    }
    temp2 = XOR(rail->tail->XORpointer, NULL);
    rail->tail->XORpointer = XOR(temp1, temp2);
    rail->tail = temp1;
    return;
}
void leave(Rail *rail){
    XORnode *temp1, *temp2;
    if(rail->head == NULL) return;
    temp1 = XOR(rail->tail->XORpointer, NULL);
    if(temp1 != NULL){
        temp2 = XOR(temp1->XORpointer, rail->tail);
        temp1->XORpointer = XOR(temp2, NULL);
    }
    else{
        free(rail->tail);
        rail->head = NULL;
        rail->tail = NULL;
        return;
    }
    free(rail->tail);
    rail->tail = temp1;
    return;
}
void migrate(Rail *r_a, Rail *r_b){
    XORnode *temp1, *temp2;
    if(r_a->tail == NULL) return;
    if(r_b->tail == NULL)
        r_b->head = r_a->tail;
    else{
        temp1 = XOR(r_b->tail->XORpointer, NULL);
        r_b->tail->XORpointer = XOR(temp1, r_a->tail);
        temp2 = XOR(r_a->tail->XORpointer, NULL);
        r_a->tail->XORpointer = XOR(temp2, r_b->tail);
    }
    r_b->tail = r_a->head;
    r_a->head = NULL;
    r_a->tail = NULL;
    return;
}
int main(void){
    short r; 
    int n;
    short rail_index1, rail_index2, cobinLabel;
    char command[9];
    scanf("%hd %d", &r, &n);

    Rail *rail[r];
    for(short i = 0; i < r; i++){
        rail[i] = (Rail *)malloc(sizeof(Rail));
        assert(rail[i] != NULL);
        rail[i]->head = NULL;
        rail[i]->tail = NULL;
    }
    for(int i = 0; i < n; i++){
        scanf("%s", command);
        /*Enter*/
        if(command[0] == 'e'){
            scanf("%hd %hd", &rail_index1, &cobinLabel);
            enter(rail[rail_index1], cobinLabel);
        }
        /*Leave*/
        else if(command[0] == 'l'){
            scanf("%hd", &rail_index1);
            leave(rail[rail_index1]);
        }
        /*Migrate*/
        else if(command[0] == 'm'){
            scanf("%hd %hd", &rail_index1, &rail_index2);
            migrate(rail[rail_index1], rail[rail_index2]);
        }
    }
    for(short i = 0; i < r; i++){
        if(rail[i]->head == NULL){
            printf("\n");
            continue;
        }
        XORnode *cur = rail[i]->head;
        XORnode *prev = NULL;
        XORnode *next = XOR(cur->XORpointer, prev);
        while(cur != NULL){
            if(next != NULL)
                printf("%hd ", cur->cobinLabel);
            else printf("%hd\n", cur->cobinLabel);
            prev = cur;
            cur = next;
            if(cur == NULL) break;
            next = XOR(cur->XORpointer, prev);
        }
    }
    return 0;
}