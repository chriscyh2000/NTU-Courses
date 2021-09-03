#include "max.h"
int max(int array[5][5]){
    int maxnum = array[0][0];
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 5; j++){
            if (array[i][j] > maxnum)
                maxnum = array[i][j];
        }
    return maxnum;
}