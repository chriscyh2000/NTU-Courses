#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void sumOfIntegers(const int *ptrArray[], int answer[], int *numberOfBlocks){
    int count = 0;
    int i = 0;
    while(ptrArray[i]){
        answer[count] = 0;
        int start = i;
        while(ptrArray[i]){
            bool test = false;
            for(int k = start; k < i; ++k){
                if(ptrArray[k] == ptrArray[i]){
                     test = true;
                     break;
                }
            }
            if(test){
                ++i;
                continue;
            }
            answer[count] += *(ptrArray[i]);
            ++i;
        }
        ++i;
        ++count;
    }
    (*numberOfBlocks) = count;
}
