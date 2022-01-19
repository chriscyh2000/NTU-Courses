#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <fcntl.h>
#include <stdbool.h>

char *id;
int depth, lucky_num;
void forkPlayer(FILE **pipe_r, int player1, int player2){
    int pipe_rfd[2][2];
    char player_str[2][64] = {"", ""};
    sprintf(player_str[0], "%d", player1);
    sprintf(player_str[1], "%d", player2);
    pipe(pipe_rfd[0]);
    if(fork() == 0){
        close(pipe_rfd[0][0]);
        dup2(pipe_rfd[0][1], STDOUT_FILENO);
        close(pipe_rfd[0][1]);
        execl("./player", "player", "-n", player_str[0], NULL);
    }
    close(pipe_rfd[0][1]);
    pipe(pipe_rfd[1]);
    if(fork() == 0){
        close(pipe_rfd[0][0]);
        close(pipe_rfd[1][0]);
        dup2(pipe_rfd[1][1], STDOUT_FILENO);
        close(pipe_rfd[1][1]);
        execl("./player", "player", "-n", player_str[1], NULL);
    }
    close(pipe_rfd[1][1]);
    pipe_r[0] = fdopen(pipe_rfd[0][0], "r");
    pipe_r[1] = fdopen(pipe_rfd[1][0], "r");
}
void myFork(int depth, FILE **pipe_w, FILE **pipe_r, int *player_id){
    int pipe_wfd[2][2], pipe_rfd[2][2];
    char dep_str[64], player_str[2][64] = {"", ""};
    char lucky_str[32];
    sprintf(lucky_str, "%d", lucky_num);
    sprintf(dep_str, "%d", depth);
    for(int i = 0; i < 2; ++i){
        pipe(pipe_wfd[i]);
        pipe(pipe_rfd[i]);
        if(fork() == 0){
            close(pipe_wfd[i][1]);
            close(pipe_rfd[i][0]);
            dup2(pipe_wfd[i][0], STDIN_FILENO);
            dup2(pipe_rfd[i][1], STDOUT_FILENO);
            close(pipe_wfd[i][0]);
            close(pipe_rfd[i][1]);
            execl("./host", "host", "-m", id, "-d", dep_str, "-l", lucky_str, NULL);
        }
        close(pipe_wfd[i][0]);
        close(pipe_rfd[i][1]);
    }
    for(int i = 0; i < 2; ++i){
        pipe_w[i] = fdopen(pipe_wfd[i][1], "w");
        pipe_r[i] = fdopen(pipe_rfd[i][0], "r");
        setbuf(pipe_w[i], NULL);
    }
}
int main(int argc, char *const argv[]){
    // check the input format
    for(int i = 1; i < 7; i += 2){
        if(argv[i][1] == 'm'){
            id = argv[i+1];
        }
        else if(argv[i][1] == 'l'){
            lucky_num = atoi(argv[i+1]);
        }
        else{
            depth = atoi(argv[i+1]);
        }
    }
    // process start
    if(depth == 0){ //root host
        FILE *pipe_w[2], *pipe_r[2];
        FILE *fifo_w, *fifo_r;
        char fifo_name[64] = "";
        sprintf(fifo_name, "fifo_%s.tmp", id);
        fifo_r = fopen(fifo_name, "r");
        int fifo0_fd = open("fifo_0.tmp", O_WRONLY);
        fifo_w = fopen("fifo_0.tmp", "w");
        myFork(1, pipe_w, pipe_r, NULL);
        while(1){
            int player_id[8] = {0};
            for(int i = 0; i < 8; ++i){
                fscanf(fifo_r, "%d", &(player_id[i]));
            }
            if(player_id[0] == -1){
                fprintf(pipe_w[0], "-1 -1 -1 -1\n");
                fprintf(pipe_w[1], "-1 -1 -1 -1\n");
                break;
            }
            char buf[64] = "";
            sprintf(buf, "%d %d %d %d\n", player_id[0], player_id[1], player_id[2], player_id[3]);
            fprintf(pipe_w[0], "%s", buf);
            sprintf(buf, "%d %d %d %d\n", player_id[4], player_id[5], player_id[6], player_id[7]);
            fprintf(pipe_w[1], "%s", buf);
            fflush(pipe_w[0]);
            fflush(pipe_w[1]);
            int score[14];
            for(int i = 0; i < 8; ++i){
                score[player_id[i]] = 0;
            }
            for(int i = 0; i < 10; ++i){
                int winner[2], guess[2];
                for(int j = 0; j < 2; ++j){
                    fscanf(pipe_r[j], "%d%d", &(winner[j]), &(guess[j]));
                }
                int champ_id;
                if(abs(guess[0]-lucky_num) < abs(guess[1]-lucky_num)){
                    champ_id = winner[0];
                }
                else if(abs(guess[0]-lucky_num) > abs(guess[1]-lucky_num)){
                    champ_id = winner[1];
                }
                else champ_id = (winner[0] < winner[1])? winner[0] : winner[1];
                score[champ_id] += 10;
            }

            flock(fifo0_fd, LOCK_EX);
            fprintf(fifo_w, "%s\n", id);
            for(int i = 0; i < 8; ++i){
                fprintf(fifo_w, "%d %d\n", player_id[i], score[player_id[i]]);
            }
            fflush(fifo_w);
            flock(fifo0_fd, LOCK_NB);
        }
    }
    else if(depth == 1){
        FILE *pipe_w[2], *pipe_r[2];
        myFork(2, pipe_w, pipe_r, NULL);
        while(1){
            int player_id[4] = {0};
            for(int i = 0; i < 4; ++i){
                scanf("%d", &(player_id[i]));
            }
            if(player_id[0] == -1){
                // fprintf(stderr, "-1 has been written\n");
                fprintf(pipe_w[0], "-1 -1\n");
                fprintf(pipe_w[1], "-1 -1\n");
                break;
            }
            char buf[64] = "";
            sprintf(buf, "%d %d\n", player_id[0], player_id[1]);
            fprintf(pipe_w[0], "%s", buf);
            sprintf(buf, "%d %d\n", player_id[2], player_id[3]);
            fprintf(pipe_w[1], "%s", buf);
            fflush(pipe_w[0]);
            fflush(pipe_w[1]);
            for(int i = 0; i < 10; ++i){
                int winner[2], guess[2];
                for(int j = 0; j < 2; ++j){
                    fscanf(pipe_r[j], "%d%d", &(winner[j]), &(guess[j]));
                }
                int champ_id, champ_guess;
                if(abs(guess[0]-lucky_num) < abs(guess[1]-lucky_num)){
                    champ_id = winner[0];
                    champ_guess = guess[0];
                }
                else if(abs(guess[0]-lucky_num) > abs(guess[1]-lucky_num)){
                    champ_id = winner[1];
                    champ_guess = guess[1];
                }
                else{
                    champ_id = (winner[0] < winner[1])? winner[0] : winner[1];
                    champ_guess = (winner[0] < winner[1])? guess[0] : guess[1];
                } 
                printf("%d %d\n", champ_id, champ_guess);
                fflush(stdout);
            }
        }
    }
    else{ // leaf host
        while(1){
            int player_id[2] = {0};
            for(int i = 0; i < 2; ++i){
                scanf("%d", &(player_id[i]));
            }
            if(player_id[0] == -1) break;
            FILE *pipe_r[2];
            // fprintf(stderr, "ready to fork player\n");
            forkPlayer(pipe_r, player_id[0], player_id[1]);
            for(int i = 0; i < 10; ++i){
                int winner[2], guess[2];
                for(int j = 0; j < 2; ++j){
                    fscanf(pipe_r[j], "%d%d", &(winner[j]), &(guess[j]));
                }
                int champ_id, champ_guess;
                if(abs(guess[0]-lucky_num) < abs(guess[1]-lucky_num)){
                    champ_id = winner[0];
                    champ_guess = guess[0];
                }
                else if(abs(guess[0]-lucky_num) > abs(guess[1]-lucky_num)){
                    champ_id = winner[1];
                    champ_guess = guess[1];
                }
                else{
                    champ_id = (winner[0] < winner[1])? winner[0] : winner[1];
                    champ_guess = (winner[0] < winner[1])? guess[0] : guess[1];
                } 
                printf("%d %d\n", champ_id, champ_guess);
                fflush(stdout);
            }
            pid_t status;
            while(wait(&status) > 0);
        }
    }
    pid_t status;
    while(wait(&status) > 0);
    return 0;
}