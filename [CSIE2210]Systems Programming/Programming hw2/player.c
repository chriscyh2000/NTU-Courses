#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    int player_id = atoi(argv[2]);
    for(int i = 1; i <= 10; ++i){
        int guess = 0;
        /* initialize random seed: */
        srand ((player_id + i) * 323);
        /* generate guess between 1 and 1000: */
        guess = rand() % 1001;
        printf("%d %d\n", player_id, guess);
    }
    return 0;
}