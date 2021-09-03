#include <stdio.h>
#include <stdbool.h>
int main(void){
    unsigned short a[40000];
    unsigned short top = 2;
    unsigned short maxlen = 0, leftLen = 0, rightLen = 2, maxStart, maxLeftLen = 0;
    unsigned short temp = 0, start = 0, mid;
    unsigned short input;
    bool existence = false;
    if(scanf("%hd", &input) != EOF) a[0] = input;
    if(scanf("%hd", &input) != EOF) a[1] = input;
    while(scanf("%hd", &input) != EOF){
        a[top] = input;//remember to add 1 to top
        if((a[top] > a[top-1] && a[top-1] > a[top-2]) || (a[top] < a[top-1] && a[top-1] < a[top-2])){
            ++rightLen;
            if(leftLen){
                if(leftLen + rightLen > maxlen){
                    maxlen = leftLen + rightLen;
                    maxStart = start;
                    maxLeftLen = leftLen;
                }
                else if(leftLen > maxLeftLen){
                    maxlen = leftLen + rightLen;
                    maxStart = start;
                }
            }
        }
        else{
            if(!leftLen){
                leftLen = rightLen;
                maxLeftLen = rightLen;
                rightLen = 1;
                mid = top - 1;
                start = 0;
                maxStart = 0;
                maxlen = leftLen + 1;  
                // printf("maxLeftlen = %d maxlen = %d\n", maxLeftLen, maxlen);
            }
            else{
                leftLen = rightLen + 1;
                rightLen = 1;
                start = mid;
                mid = top - 1;
                if(maxlen < leftLen + 1){
                    maxlen = leftLen + 1;
                    maxStart = start;
                    maxLeftLen = leftLen;
                }
                else if(leftLen > maxLeftLen){
                    maxlen = leftLen + rightLen;
                    maxStart = start;
                }
                // printf("maxLeftlen = %d maxlen = %d\n", maxLeftLen, maxlen);
            }
        }
        ++top;
    }
    if(top < 3 || !leftLen) printf("0\n");
    else printf("%d %d\n", maxlen, maxStart + 1);
}