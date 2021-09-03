#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAXSIZE 2000
int size;
int prevSize;
int numOfData;

typedef struct node{
    bool reverseTag;
    bool sorted;
    int arr[MAXSIZE];
    int sortedArr[MAXSIZE];
    int noe;
    struct node *ptr;
}Node;

int compare(const void *data1, const void *data2){
    int *ptr1 = (int *)data1;
    int *ptr2 = (int *)data2;
    if(*ptr1 < *ptr2) return -1;
    if(*ptr1 > *ptr2) return 1;
    return 0;
}

Node *XOR(Node *a, Node *b){
    Node *temp1;
    temp1 = (Node *)((intptr_t)(a) ^ (intptr_t)(b));
    return temp1;
}

Node *genListNode(bool reverseTag, Node *ptr){
    Node *temp = (Node *)malloc(sizeof(Node));
    assert(temp != NULL);
    temp->noe = 0;
    temp->reverseTag = reverseTag;
    temp->sorted = false;
    temp->ptr = ptr;
    return temp;
}

void reverseNode(Node *cur){
    if (cur->noe == 0) return;
    cur->reverseTag = false;
    int temp;
    for(int i = 0; i < cur->noe / 2; i++){
        temp = cur->arr[i];
        cur->arr[i] = cur->arr[cur->noe-i-1];
        cur->arr[cur->noe-i-1] = temp;
    }
    return;
}

void insert(int i, int x, Node **head){
    numOfData++;
    prevSize = size;
    size = numOfData / 50 + 50;

    if(*head == NULL){
        *head = genListNode(false, NULL);
        (*head)->arr[(*head)->noe++] = x;
        return;
    }
    Node *prev = NULL;
    Node *cur = *head;
    Node *next = XOR((*head)->ptr, NULL);
    while(i > cur->noe + 1){
        i -= cur->noe;
        prev = cur;
        cur = next;
        if(cur == NULL) break;
        next = XOR(cur->ptr, prev);
    }
    cur->sorted = false;
    if(cur->reverseTag == true) reverseNode(cur);

    if(cur->noe >= size){
        cur->ptr = XOR(prev, genListNode(false, XOR(cur, next)));
        if(next != NULL)
            next->ptr = XOR(XOR(cur->ptr, prev), XOR(cur, next->ptr));
        next = XOR(prev, cur->ptr);
        int end = cur->noe;
        for(int k = cur->noe / 2 + 1; k < end; k++){
            next->arr[next->noe++] = cur->arr[k];
            cur->noe--;
        }
        if(i > cur->noe){
            i -= cur->noe;
            cur = next;
        }
    }
    if(i == cur->noe + 1){
        cur->arr[cur->noe++] = x;
        return;
    }
    for(int k = cur->noe - 1; k >= i - 1; k--)
        cur->arr[k+1] = cur->arr[k];
    cur->arr[i-1] = x;
    cur->noe++;
    return;
}

void delete(int i, Node **head){
    numOfData--;
    prevSize = size;
    size = numOfData / 50 + 50;
    
    Node *prev = NULL;
    Node *cur = *head;
    Node *next = XOR(cur->ptr, prev);
    while(i > cur->noe){
        i -= cur->noe;
        prev = cur;
        cur = next;
        if(cur == NULL) break;
        next = XOR(cur->ptr, prev);
    }

    if(cur->noe == 1){
        if(prev == NULL){
            Node *temp = XOR(cur->ptr, prev);
            if(temp != NULL)
                temp->ptr = XOR(NULL, XOR(cur, temp->ptr));
            *head = temp;
        }
        else{
            prev->ptr = XOR(XOR(prev->ptr, cur), next);
            if(next != NULL)
                next->ptr = XOR(XOR(next->ptr, cur), prev);
        }
        free(cur);
        return;
    }

    cur->sorted = false;

    if(cur->reverseTag) reverseNode(cur);
    for(int k = i - 1; k < cur->noe - 1; k++)
        cur->arr[k] = cur->arr[k+1];
    cur->noe--;

    if(next == NULL) return;
    if(cur->noe >= size / 2 + 1) return;
    next->sorted = false;
    if(!next->reverseTag){
        int j = 0;
        while(cur->noe < size / 2 + 1 && next->noe > 0){
            cur->arr[cur->noe++] = next->arr[j++];
            next->noe--;
        }
        if(next->noe == 0){
            Node *temp = XOR(next->ptr, cur);
            cur->ptr = XOR(temp, prev);
            if(temp != NULL)
                temp->ptr = XOR(XOR(temp->ptr, next), cur);
            free(next);
            return;
        }

        if(next->noe >= size / 2 + 1){
            for(int k = 0; k < next->noe; k++)
                next->arr[k] = next->arr[k+j];
        }
        else if(next->noe < size / 2 + 1){
            while(next->noe != 0){
                cur->arr[cur->noe++] = next->arr[j++];
                next->noe--;
            }
            Node *temp = XOR(next->ptr, cur);
            cur->ptr = XOR(temp, prev);
            if(temp != NULL)
                temp->ptr = XOR(XOR(temp->ptr, next), cur);
            free(next);
            return;
        }
    }
    else if(next->reverseTag){
        int j = next->noe - 1;
        while(cur->noe < size / 2 + 1&& next->noe > 0){
            cur->arr[cur->noe++] = next->arr[j--];
            next->noe--;
        }
        if(next->noe < size / 2 + 1){
            while(next->noe != 0){
                cur->arr[cur->noe++] = next->arr[j--];
                next->noe--;
            }
            Node *temp = XOR(next->ptr, cur);
            cur->ptr = XOR(temp, prev);
            if(temp != NULL)
                temp->ptr = XOR(XOR(temp->ptr, next), cur);
            free(next);
            return;
        }
    }
    return;
}

void reverse(int left, int right, Node **head){
    if(left == right) return;
    Node *prev = NULL;
    Node *cur = *head;
    Node *next = XOR(cur->ptr, prev);
    Node *leftNode, *leftPrev, *rightNode, *rightPrev;
    Node *leftNext, *rightNext;
    while(left > cur->noe){
        left -= cur->noe;
        right -= cur->noe;
        prev = cur;
        cur = next;
        next = XOR(cur->ptr, prev);
    }
    leftNode = cur;
    leftPrev = prev;
    leftNext = next;
    while(right > cur->noe){
        right -= cur->noe;
        prev = cur;
        cur = next;
        next = XOR(cur->ptr, prev);
    }
    rightNode = cur; 
    rightPrev = prev;
    rightNext = next;
    if(leftNode == rightNode){
        if(rightNode->reverseTag) reverseNode(rightNode);
        if(left == 1 && right == rightNode->noe)
            rightNode->reverseTag = true;
        else if(left != 1 && right != rightNode->noe){
            rightNode->sorted = false;
            if(rightNode->reverseTag) reverseNode(rightNode);
            Node *temp = genListNode(true, NULL);
            Node *temp2 = genListNode(false, NULL);
            temp->ptr = XOR(cur, temp2);
            cur->ptr = XOR(prev, temp);
            temp2->ptr = XOR(temp, rightNext);
            if(rightNext != NULL)
                rightNext->ptr = XOR(temp2, XOR(rightNode, rightNext->ptr));
            for(int i = left - 1; i < right; i++)
                temp->arr[temp->noe++] = rightNode->arr[i];
            for(int i = right; i < rightNode->noe; i++)
                temp2->arr[temp2->noe++] = rightNode->arr[i];
            rightNode->noe = left - 1;
        }
        else if(left != 1 && right == rightNode->noe){
            rightNode->sorted = false;
            Node *temp = genListNode(true, XOR(rightNode, rightNext));
            rightNode->ptr = XOR(rightPrev, temp);
            for(int i = left - 1; i < right; i++)
                temp->arr[temp->noe++] = rightNode->arr[i];
            if(rightNext != NULL)
                rightNext->ptr = XOR(temp, XOR(rightNext->ptr, cur));
            rightNode->noe = left - 1;
        }
        else if(left == 1 && right != rightNode->noe){
            rightNode->sorted = false;
            Node *temp = genListNode(false, XOR(rightNode, rightNext));
            for(int i = right; i < rightNode->noe; i++)
                temp->arr[temp->noe++] = rightNode->arr[i];
            cur->ptr = XOR(prev, temp);
            if(next != NULL)
                rightNext->ptr = XOR(temp, XOR(rightNext->ptr, cur));
            rightNode->noe = right - left + 1;
            rightNode->reverseTag = !(rightNode->reverseTag);
        }
    }
    else{
        if(left == 1)
            leftNode->reverseTag = !(leftNode->reverseTag);
        else{
            if(leftNode->reverseTag) reverseNode(leftNode);
            leftNode->sorted = false;

            Node *temp = genListNode(true, XOR(leftNode, leftNext));
            for(int i = left - 1; i < leftNode->noe; i++)
                temp->arr[temp->noe++] = leftNode->arr[i];
            leftNode->noe = left -1;

            leftNode->ptr = XOR(leftPrev, temp);
            if(leftNext != NULL)
                leftNext->ptr = XOR(temp, XOR(leftNode, leftNext->ptr));
            if(leftNode == rightPrev) rightPrev = temp;
            leftPrev = leftNode;
            leftNode = temp;
        }
        if(right == rightNode->noe)
            rightNode->reverseTag = !(rightNode->reverseTag);
        else{
            if(rightNode->reverseTag) reverseNode(rightNode);
            rightNode->sorted = false;

            Node *temp = genListNode(false, XOR(rightNode, rightNext));
            for(int i = right; i < rightNode->noe; i++)
                temp->arr[temp->noe++] = rightNode->arr[i];
            rightNode->noe = right;
            rightNode->reverseTag = true;

            rightNode->ptr = XOR(rightPrev, temp);
            if(rightNext != NULL)
                rightNext->ptr = XOR(temp, XOR(rightNode, rightNext->ptr));
            
            rightNext = temp;
        }
        leftNode->ptr = XOR(leftNext, rightNext);
        rightNode->ptr = XOR(leftPrev, rightPrev);
        if(leftPrev != NULL)
            leftPrev->ptr = XOR(XOR(leftPrev->ptr, leftNode), rightNode);
        else *head = rightNode;
        if(rightNext != NULL)
            rightNext->ptr = XOR(XOR(rightNext->ptr, rightNode), leftNode);

        prev = rightNode;
        cur = rightPrev;
        next = XOR(cur->ptr, prev);
        while(cur != leftNode){
            next = XOR(cur->ptr, prev);
            cur->reverseTag = !(cur->reverseTag);
            prev = cur;
            cur = next;
        }
    }
    return;
}

void count(int arr[], int arraySize, int *countLess, int *countEqual, int num){
    if(num < arr[0]) return;
    if(num > arr[arraySize-1]){
        (*countLess) += arraySize;
        return;
    }
    int upBound = arraySize - 1;
    int lowBound = 0;
    int midPoint;
    while(lowBound <= upBound){
        midPoint = (lowBound + upBound) / 2;
        if(arr[midPoint] == num){
            int inUpBound, inLowBound, inMidPoint;
            int lowBorder = midPoint, upBorder = midPoint;
            bool check = true;
            inUpBound = midPoint - 1;
            inLowBound = lowBound;
            while(check){
                while(inLowBound <= inUpBound){
                    inMidPoint = (inUpBound + inLowBound) / 2;
                    if(arr[inMidPoint] == num){
                        lowBorder = inMidPoint;
                        break;
                    }
                    else if(arr[inMidPoint] < num)
                        inLowBound = inMidPoint + 1;
                }
                if(inLowBound > inUpBound)
                    check = false;
                else inUpBound = lowBorder - 1;
            }
            (*countLess) += lowBorder;
            check = true;
            inUpBound = upBound;
            inLowBound = midPoint + 1;
            while(check){
                while(inLowBound <= inUpBound){
                    inMidPoint = (inUpBound + inLowBound) / 2;
                    if(arr[inMidPoint] == num){
                        upBorder = inMidPoint;
                        break;
                    }
                    else if(arr[inMidPoint] > num)
                        inUpBound = inMidPoint - 1;
                }
                if(inLowBound > inUpBound)
                    check = false;
                else inLowBound = upBorder + 1;
            }
            (*countEqual) += (upBorder - lowBorder + 1);
            break;
        }
        else if(arr[midPoint] > num)
            upBound = midPoint - 1;
        else if(arr[midPoint] < num)
            lowBound = midPoint + 1;
    }
    if(lowBound > upBound)
        (*countLess) += lowBound;
    return;
}

int k_least(Node *leftNode, Node *rightNode, Node *leftNext,
            int k, int left, int right){
    int ans;
    if(leftNode == rightNode){
        if(left == right && !rightNode->reverseTag) return rightNode->arr[right-1];
        if(left == right && rightNode->reverseTag) 
            return rightNode->arr[rightNode->noe-right];
        if(left == 1 && right == rightNode->noe && rightNode->sorted)
            return rightNode->sortedArr[k-1];
        int *searchArray = calloc(right - left + 1, sizeof(int));
        if(!rightNode->reverseTag)
            memcpy(searchArray, &rightNode->arr[left-1], (right - left + 1) * sizeof(int));
        else
            memcpy(searchArray, &rightNode->arr[rightNode->noe-right], (right - left + 1) * sizeof(int));
        qsort(searchArray, right - left + 1, sizeof(int), compare);
        ans = searchArray[k-1];
        free(searchArray);
    }
    else{
        int arraySize_l = leftNode->noe - left + 1;
        int arraySize_r = right;
        int *searchArray_l = calloc(arraySize_l, sizeof(int));
        int *searchArray_r = calloc(arraySize_r, sizeof(int));
        if(!leftNode->reverseTag)
            memcpy(searchArray_l, &leftNode->arr[left-1], arraySize_l * sizeof(int));
        else 
            memcpy(searchArray_l, leftNode->arr, arraySize_l * sizeof(int));
        if(!rightNode->reverseTag)
            memcpy(searchArray_r, rightNode->arr, arraySize_r * sizeof(int));
        else    
            memcpy(searchArray_r, &rightNode->arr[rightNode->noe-right], arraySize_r * sizeof(int));
        qsort(searchArray_l, leftNode->noe - left + 1, sizeof(int), compare);
        qsort(searchArray_r, right, sizeof(int), compare);
        int min = -100000, max = 100000, mid;
        int countLess, countEqual;
        Node *prev, *cur, *next;
        while(min <= max){
            mid = (min + max) / 2;
            countLess = 0;
            countEqual = 0;
            prev = leftNode;
            cur = leftNext;
            next = XOR(cur->ptr, prev);
            while(cur != rightNode){
                next = XOR(cur->ptr, prev);
                count(cur->sortedArr, cur->noe, &countLess, &countEqual, mid);
                if(countLess >= k) break;
                prev = cur;
                cur = next;
            }
            count(searchArray_l, arraySize_l, &countLess, &countEqual, mid);
            count(searchArray_r, arraySize_r, &countLess, &countEqual, mid);
            
            if(countLess < k && (countLess + countEqual) >= k) {
                ans = mid;
                break;
            }
            else if(countLess >= k) max = mid - 1;
            else if(countLess < k && countLess + countEqual < k) min = mid + 1;
        }
        free(searchArray_l);
        free(searchArray_r);
    }
    return ans;
}

int query(int left, int right, int k, Node *head){
    Node *prev = NULL;
    Node *cur = head;
    Node *next = XOR(cur->ptr, prev);
    Node *leftNode, *leftNext, *rightNode;
    while(left > cur->noe){
        left -= cur->noe;
        right -= cur->noe;
        prev = cur;
        cur = next;
        next = XOR(cur->ptr, prev);
    }
    leftNode = cur;
    leftNext = next;

    while(right > cur->noe){
        if(!cur->sorted && cur != leftNode){
            cur->sorted = true;
            memcpy(cur->sortedArr, cur->arr, cur->noe * sizeof(int));
            qsort(cur->sortedArr, cur->noe, sizeof(int), compare);
        }
        right -= cur->noe;
        prev = cur;
        cur = next;
        next = XOR(cur->ptr, prev);
    }
    rightNode = cur; 
    return k_least(leftNode, rightNode, leftNext, k, left, right);
}
void adjust(Node *head){
    if(head == NULL) return;
    Node *prev, *cur, *next;
    prev = NULL;
    cur = head;
    next = XOR(cur->ptr, prev);
    while(cur != NULL){
        next = XOR(cur->ptr, prev);
        if(cur->noe < size / 2 + 1 && next != NULL){
            if(cur->reverseTag) reverseNode(cur);
            cur->sorted = false;
            next->sorted = false;

            if(!next->reverseTag){
                int j = 0;
                while(cur->noe < size / 2 + 1 && next->noe > 0){
                    cur->arr[cur->noe++] = next->arr[j++];
                    next->noe--;
                }
                if(next->noe == 0){
                    Node *temp = XOR(next->ptr, cur);
                    cur->ptr = XOR(temp, prev);
                    if(temp != NULL)
                        temp->ptr = XOR(XOR(temp->ptr, next), cur);
                    free(next);
                    next = temp;
                }
                else if(next->noe >= size / 2 + 1){
                    for(int k = 0; k < next->noe; k++)
                        next->arr[k] = next->arr[k+j];
                }
                else if(next->noe < size / 2 + 1){
                    while(next->noe != 0){
                        cur->arr[cur->noe++] = next->arr[j++];
                        next->noe--;
                    }
                    Node *temp = XOR(next->ptr, cur);
                    cur->ptr = XOR(temp, prev);
                    if(temp != NULL)
                        temp->ptr = XOR(XOR(temp->ptr, next), cur);
                    free(next); 
                    next = temp;
                }
            }
            else if(next->reverseTag){
                int j = next->noe - 1;
                while(cur->noe < size / 2 + 1 && next->noe > 0){
                    cur->arr[cur->noe++] = next->arr[j--];
                    next->noe--;
                }
                if(next->noe < size / 2 + 1){
                    while(next->noe != 0){
                        cur->arr[cur->noe++] = next->arr[j--];
                        next->noe--;
                    }
                    Node *temp = XOR(next->ptr, cur);
                    cur->ptr = XOR(temp, prev);
                    if(temp != NULL)
                        temp->ptr = XOR(XOR(temp->ptr, next), cur);
                    free(next);
                    next = temp;
                }
            }
        }
        else if(cur->noe >= size){
            cur->sorted = false;
            if(cur->reverseTag) reverseNode(cur);
            Node *temp = genListNode(false, XOR(cur, next));
            cur->ptr = XOR(prev, temp);
            if(next != NULL)
                next->ptr = XOR(temp, XOR(cur, next->ptr));
            next = temp;
            int end = cur->noe;
            for(int k = cur->noe / 2 + 1; k < end; k++){
                next->arr[next->noe++] = cur->arr[k];
                cur->noe--;
            }        
        }
        prev = cur;
        cur = next;
    }
}

int main(void){
    char command[9];
    int i, l, r, k;
    int x;
    Node *head = NULL;
    Node *prev = NULL, *cur, *next;
    numOfData = 0;
    int n, q;
    scanf("%d%d", &n, &q);
    size = n / 50 + 50;
    int numOfinitialNodes = n / (size * 8 / 10) + 1;
    for(int u = 0; u < numOfinitialNodes; u++){
        if(numOfData == n) break;
        cur = genListNode(false, XOR(prev, NULL));
        if(head == NULL) head = cur;
        if(prev != NULL) prev->ptr = XOR(cur, XOR(NULL, prev->ptr));
        for(int i = 0; i < (size * 8 / 10) && numOfData < n; i++){
            scanf("%d", &x);
            cur->arr[cur->noe++] = x;
            numOfData++;
        }
        prev = cur;
    }
    for(int u = 0; u < q; u++){
        scanf("%s", command);
        if(command[0] == 'I'){
            scanf("%d%d", &i, &x);
            insert(i, x, &head);
        }
        else if(command[0] == 'D'){
            scanf("%d", &i);
            delete(i, &head);
        }
        else if (command[0] == 'R'){
            scanf("%d%d", &l, &r);
            reverse(l, r, &head);
        }
        else if(command[0] == 'Q'){
            scanf("%d%d%d", &l, &r, &k);
            printf("%d\n", query(l, r, k, head));
        }
        if(((command[0] == 'I' || command[0] == 'D') && size != prevSize) || command[0] == 'R')
            adjust(head);
    }
    return 0;
}
