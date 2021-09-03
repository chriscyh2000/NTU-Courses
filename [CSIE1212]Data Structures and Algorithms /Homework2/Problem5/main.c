/* Only discussed with TAs. */
#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
typedef struct graph{
    int p;
    int l;
    int r;
}Triangle;
int compare(const void *data1, const void *data2){
    Triangle *ptr1 = (Triangle *)data1;
    Triangle *ptr2 = (Triangle *)data2;
    int p1 = ptr1->p, p2 = ptr2->p;
    if(p1 != p2)
        return p1 - p2;
    return ptr2->r - ptr1->r;
}
void merge(long long *count, int L1[], int L2[], int R1[], int R2[], 
           int L_arr[], int R_arr[], int size1, int size2){
    int i = 0, j = 0, k = 0;
    while(i < size1 && j < size2){
        if(L2[j] <= R1[i]){
            (*count) += size1 - i;
            ++j;
        }
        else ++i;
    }

    i = 0; j = 0;
    while(i < size1 && j < size2){
        if(L1[i] <= L2[j])
            L_arr[k++] = L1[i++];
        else
            L_arr[k++] = L2[j++];
    }
    while(i < size1)
        L_arr[k++] = L1[i++];
    while(j < size2)
        L_arr[k++] = L2[j++];
    i = 0; j = 0; k = 0;
    while(i < size1 && j < size2){
        if(R1[i] <= R2[j])
            R_arr[k++] = R1[i++];
        else
            R_arr[k++] = R2[j++];
    }
    while(i < size1)
        R_arr[k++] = R1[i++];
    while(j < size2)
        R_arr[k++] = R2[j++];
    return;
}
void mergeSort(long long *count, int L_arr[], int R_arr[], int size){
    if(size == 1) return;
    int size1 = size / 2;
    int size2 = size - size1;
    int L1[size1], R1[size1];
    int L2[size2], R2[size2];
    
    for(int i = 0; i < size1; ++i){
        L1[i] = L_arr[i];
        R1[i] = R_arr[i];
    }
    for(int j = 0, k = size1; j < size2; ++j, ++k){
        L2[j] = L_arr[k];
        R2[j] = R_arr[k];
    }

    mergeSort(count, L1, R1, size1);
    mergeSort(count, L2, R2, size2);
    merge(count, L1, L2, R1, R2, L_arr, R_arr, size1, size2);
    return;
}
Triangle triangle[3000000];
int L[3000000], R[3000000];
int main(void) {
    generator.init();
    int t = generator.getT();
    long long count;
    while (t--) {
        int n, *p, *q, *r;
        int u, v;
        generator.getData(&n, &p, &q, &r);
        for(int i = 0; i < n; ++i){
            triangle[i].p = p[i];
            u = q[i];
            v = r[i]; 
            triangle[i].l = (u < v) ? u : v;
            triangle[i].r = (u < v) ? v : u;
        }
        qsort(triangle, n, sizeof(Triangle), compare);
        for(int i = 0; i < n; ++i){
            L[i] = triangle[i].l;
            R[i] = triangle[i].r;
        }
        count = 0;
        mergeSort(&count, L, R, n);
        printf("%lld\n", count);
    }
    return 0;
}