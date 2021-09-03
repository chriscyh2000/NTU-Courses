#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct DisjointSet{    
    // TODO: Determine fields to use by your method
    int parent_idx;
    int degree;
} disjoint_set;
disjoint_set ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) {    
    if ('0' <= c && c <= '9') return c - '0';    
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;    
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;    
    return -1;
}
int hash(const char* s) {    
    int ret = 0;    
    for (int i = 0; i < 4; ++i)        
        ret = (ret << 6) | c2i(s[i]);    
    return ret;
}
void makeset(const char* s){ 
    int i = hash(s);
    ds[i].parent_idx = i;
    ds[i].degree = 0;
    // TODO: Initialize a set
}
inline void static init(const char* s) { 
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1; 
    }
}
int path_compressing(int i){
    if(ds[i].parent_idx == i) return i;
    ds[i].parent_idx = path_compressing(ds[i].parent_idx);
    return ds[i].parent_idx;
}
int find_set(const char* s) {
    init(s);
    int i = hash(s);
    // TODO: Implement your find algorithm here 
    ds[i].parent_idx = path_compressing(i);
    return ds[i].parent_idx;
}
void link(const char *ra, const char *rb) {
    int a = find_set(ra), b = find_set(rb);
    if(ds[a].degree >= ds[b].degree){
        ds[b].parent_idx = a;
        if(ds[b].degree == ds[a].degree)
            ++ds[a].degree;
    }
    else{
        ds[b].parent_idx = a;
    }
    // TODO: Implement your union algorithm here
}
bool same_set(const char *a, const char *b) { 
    return (find_set(a) == find_set(b));
}
bool stringcompare(const char *a, const char *b) {
    // implement your string compare
    int lenA = strlen(a), lenB = strlen(b);
    if(lenA != lenB) return false;
    for(int i = 0; i < lenA / 4; ++i)
        if(!same_set((a + i * 4), (b + i * 4))) return false;
    return true;
}
int main() {
    int n;
    scanf("%d", &n);
    char cmd[16], a[512], b[512]; 
    for (int i = 0; i < n; ++i) {
        scanf("%s %s %s", cmd, a, b); 
        if (!strcmp(cmd, "union")) {
            init(a);
            init(b);
            link(a, b);
        } else {
            bool same = stringcompare(a, b); 
            if(same) puts("True");
            else puts("False");
        } 
    }
}