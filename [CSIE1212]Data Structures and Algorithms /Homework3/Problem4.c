#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABETSIZE 58
#define ADJUST 65
#define STRLEN 100007
#define Q 37025580
void breakString(char *str, int head, int tail){
    if(head >= tail){
        if(head == tail) printf("%c", str[head]);
        return;
    }
    int p1 = 0;
    int p2 = 0;
    int multiplier = 1;
    int mid = (tail + head) / 2;
    if((tail - head + 1) % 2 == 0) ++mid;
    int i, j;
    bool check = false;
    for(i = head; i < mid; ++i){
        j = tail - (i - head);
        p1 = (p1 * ALPHABETSIZE + (str[i] - ADJUST)) % Q;
        p2 = (p2 + (str[j] - ADJUST) * multiplier) % Q;
        multiplier = (multiplier * ALPHABETSIZE) % Q;
        if(p1 == p2){
            if(strncmp(&str[head], &str[j], i - head + 1) == 0){
                check = true;
                break;
            }
        }
    }
    char temp;
    if(check){
        temp = str[i+1];
        str[i+1] = '\0';
        printf("%s", &str[head]);
        str[i+1] = temp;
        printf("|");
        breakString(str, i+1, j-1);
        if(i + 1 <= j - 1) printf("|");
        temp = str[tail+1];
        str[tail+1] = '\0';
        printf("%s", &str[j]);
        str[tail+1] = temp;
    }
    else{
        temp = str[tail+1];
        str[tail+1] = '\0';
        printf("%s", &str[head]);
        str[tail+1] = temp;
    }
    return;
}
int main(void){
    char temp[STRLEN];
    int T, D_len, G_len;
    int temp_len, min;
    int temp_i, temp_j, i, j;
    scanf("%d", &T);
    for(int n = 0; n < T; ++n){
        char D[STRLEN];
        char G[STRLEN];
        bool exist[ALPHABETSIZE] = {0};
        int charCount[ALPHABETSIZE] = {0};
        min = 2147483647;
        scanf("%s", D);
        scanf("%s", G);
        D_len = strlen(D);
        G_len = strlen(G);
        for(int m = 0; m < G_len; ++m){
            exist[G[m]-ADJUST] = true;
            ++charCount[G[m]-ADJUST];
        }
        temp_i = 0;
        temp_j = 0;
        while(G_len > 0 && temp_j < D_len){
            if(exist[D[temp_j]-ADJUST]){
                --charCount[D[temp_j]-ADJUST];
                if(charCount[D[temp_j]-ADJUST] >= 0) --G_len;
            }
            ++temp_j;
        }
        --temp_j;
        bool check = true;
        if(G_len == 0){
            i = temp_i;
            j = temp_j;
            min = temp_j - temp_i + 1;
            while(true){
                while(G_len == 0 && temp_i < temp_j){
                    if(exist[D[temp_i]-ADJUST]){
                        if(charCount[D[temp_i]-ADJUST] < 0){
                            ++charCount[D[temp_i]-ADJUST];
                            ++temp_i;
                        } 
                        else break;
                    }
                    else ++temp_i;
                    if(min > temp_j - temp_i + 1){
                        i = temp_i;
                        j = temp_j;
                        min = temp_j - temp_i + 1;
                    }
                }
                if(temp_j == D_len - 1) break;
                ++temp_j;
                ++temp_i;
                ++charCount[D[temp_i-1]-ADJUST];
                ++G_len;
                while(G_len > 0 && temp_j < D_len){
                    if(exist[D[temp_j]-ADJUST]){
                        --charCount[D[temp_j]-ADJUST];
                        if(charCount[D[temp_j]-ADJUST] >= 0) --G_len;
                    }
                    ++temp_j;
                }
                --temp_j;
            }
            if(temp_j == D_len) --temp_j;
        }
        else check = false;
        if(check){
            strcpy(temp, &D[j+1]);
            strcpy(&D[i], temp);
            D_len = strlen(D);
        }
        breakString(D, 0, D_len - 1);
        printf("\n");
    }
    return 0;
}