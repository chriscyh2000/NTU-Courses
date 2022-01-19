#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

char *board[2];
pthread_mutex_t done_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cur_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cur_cond = PTHREAD_COND_INITIALIZER;

struct sigaction act_c, act_p1, act_p2;
sigset_t blockall_mask, nonblock_mask, usr1_mask;

int cur = -1, done = 0, ep, pid;
int n, m, epoch;

typedef struct task{
    int i_st;
    int j_st;
    int i_end;
    int j_end;
}Task;

Task tsk_p;

// thread
void *nextState_t(void *args){
    Task *tsk = (Task *)args;
    int i_st = tsk->i_st, i_end = tsk->i_end;
    int j_st = tsk->j_st, j_end = tsk->j_end;
    for(int e = 0; e < epoch; ++e){
        pthread_mutex_lock(&cur_mut);
        while(cur != e){
            pthread_cond_wait(&cur_cond, &cur_mut);
        }
        pthread_mutex_unlock(&cur_mut);

        // run this thread...
        for(int i = i_st; i < i_end; ++i){
            for(int j = j_st; j < j_end; ++j){
                if(board[e%2][i*(m+1)+j] == '\n') { 
                    board[(e+1)%2][i*(m+1)+j] = '\n'; 
                    continue;
                }

                int alive = 0, dead = 0; 
                for(int x = i-1; x <= i+1; ++x){
                    for(int y = j-1; y <= j+1; ++y){
                        if(x < 0 || x >= n || y < 0 || y >= m || (x == i && y == j)) continue;

                        if(board[e%2][x*(m+1)+y] == 'O') ++alive;
                        else ++dead;
                    }
                }
                if(board[e%2][i*(m+1)+j] == 'O'){
                    if(alive < 2) board[(e+1)%2][i*(m+1)+j] = '.';
                    else if(alive <= 3) board[(e+1)%2][i*(m+1)+j] = 'O';
                    else board[(e+1)%2][i*(m+1)+j] = '.';
                }
                else{
                    if(alive == 3) board[(e+1)%2][i*(m+1)+j] = 'O';
                    else board[(e+1)%2][i*(m+1)+j] = '.';
                }
            }
        }


        pthread_mutex_lock(&done_mut);
        ++done;
        pthread_mutex_unlock(&done_mut);
        pthread_cond_signal(&done_cond);
    }
    pthread_exit((void *)1);
}

// signal handlers
void nextState_p(int signo){
    int i_st = tsk_p.i_st, i_end = tsk_p.i_end;
    int j_st = tsk_p.j_st, j_end = tsk_p.j_end;

    for(int i = i_st; i < i_end; ++i){
        for(int j = j_st; j < j_end; ++j){
            if(board[ep%2][i*(m+1)+j] == '\n') { 
                board[(ep+1)%2][i*(m+1)+j] = '\n'; 
                continue;
            }

            int alive = 0; 
            for(int x = i-1; x <= i+1; ++x){
                for(int y = j-1; y <= j+1; ++y){
                    if(x < 0 || x >= n || y < 0 || y >= m || (x == i && y == j)) continue;
                    if(board[ep%2][x*(m+1)+y] == 'O') ++alive;
                }
            }
            if(board[ep%2][i*(m+1)+j] == 'O'){
                if(alive < 2) board[(ep+1)%2][i*(m+1)+j] = '.';
                else if(alive <= 3) board[(ep+1)%2][i*(m+1)+j] = 'O';
                else board[(ep+1)%2][i*(m+1)+j] = '.';
            }
            else{
                if(alive == 3) board[(ep+1)%2][i*(m+1)+j] = 'O';
                else board[(ep+1)%2][i*(m+1)+j] = '.';
            }
        }
    }
    kill(getppid(), (pid % 2) ? SIGUSR1 : SIGUSR2);
    
    // printf("ep = %d! process[%d] kill parent process[%d] %s\n\n", ep, getpid(), getppid(), (pid % 2) ? "SIGUSR1" : "SIGUSR2");
}
void addDone(int signo){
    ++done;
}

int main(int argc, const char* argv[]){
    if (argc != 5) {
        fprintf(stderr, "format: ./main [-p or -t] [quantity] [input file] [output file]\n");
        return 0;
    }
    const char *type = argv[1];
    const char *infile = argv[3], *outfile = argv[4];
    const int q = atoi(argv[2]);

    FILE *input_fp = fopen(infile, "r");
    FILE *output_fp = fopen(outfile, "w");
    int input_fd = fileno(input_fp);
    int output_fd = fileno(output_fp);

    FILE *temp_in = fopen(infile, "r");
    char buf[256];
    fgets(buf, 256, temp_in);

    sscanf(buf, "%d %d %d", &n, &m, &epoch);
    int len;
    if (n > m) len = n / q;
    else len = (m + 1) / q;

    // printf("n: %d, m: %d, epoch: %d\n", n, m, epoch);
    // printf("len = %d\n", len);


    if(type[1] == 't'){
        board[0] = malloc(sizeof(char) * n * (m+1));
        board[1] = malloc(sizeof(char) * n * (m+1));

        lseek(input_fd, strlen(buf), SEEK_SET);
        read(input_fd, board[0], 1e9);
        board[0][n*(m+1)-1] = '\n';

        int err;
        Task *tsk[q];
        pthread_t tid[q];

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        for(int i = 0; i < q; ++i){
            tsk[i] = malloc(sizeof(Task));
            tsk[i]->i_st = (n > m) ? (i * len) : 0;
            tsk[i]->j_st = (n > m) ? 0 : (i * len);
            tsk[i]->i_end = (n > m) ? ((i+1) * len) : n;
            tsk[i]->j_end = (n > m) ? m + 1 : ((i+1) * len);
            if(i == q-1){
                tsk[i]->i_end = n;
                tsk[i]->j_end = m + 1;
            }

            err = pthread_create(&tid[i], &attr, nextState_t, tsk[i]);
            if (err != 0) {
                fprintf(stderr, "Can't create thread[%d]: %s\n", i, strerror(err));
                exit(-1);
            }
        }
        pthread_attr_destroy(&attr);

        // for(int i = 0; i < n; ++i){
        //     for(int j = 0; j <= m; ++j){
        //         printf("%c", board[0][i*(m+1)+j]);
        //     }
        // }

        for(int e = 0; e < epoch; ++e){
            if(e){
                pthread_mutex_lock(&done_mut);
                while(done < q){
                    pthread_cond_wait(&done_cond, &done_mut);
                }
                done = 0;
                pthread_mutex_unlock(&done_mut);
            }
            pthread_mutex_lock(&cur_mut);
            ++cur;
            pthread_mutex_unlock(&cur_mut);
            pthread_cond_broadcast(&cur_cond);
        }

        while(done < q){
            pthread_cond_wait(&done_cond, &done_mut);
        }
        pthread_mutex_unlock(&done_mut);

        // chmod(outfile, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);
        write(output_fd, board[epoch%2], sizeof(char) * (n * (m+1)-1));
    }
    else{
        board[0] = (char *)mmap(NULL, sizeof(char) * n * (m+1), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        board[1] = (char *)mmap(NULL, sizeof(char) * n * (m+1), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        lseek(input_fd, strlen(buf), SEEK_SET);
        read(input_fd, board[0], 1e9);
        board[0][n*(m+1)-1] = '\n';

        sigemptyset(&blockall_mask);
        sigemptyset(&nonblock_mask);
        sigaddset(&blockall_mask, SIGUSR2);
        sigprocmask(SIG_SETMASK, &blockall_mask, NULL);
        act_c.sa_handler = nextState_p;

        if(sigaction(SIGUSR2, &act_c, NULL) < 0){
            fprintf(stderr, "sigaction error.\n");
        }

        pid_t child_pid[q];

        for(int i = 0; i < q; ++i){
            tsk_p.i_st = (n > m) ? (i * len) : 0;
            tsk_p.j_st = (n > m) ? 0 : (i * len);
            tsk_p.i_end = (n > m) ? ((i+1) * len) : n;
            tsk_p.j_end = (n > m) ? m + 1 : ((i+1) * len);
            if(i == q-1){
                tsk_p.i_end = n;
                tsk_p.j_end = m + 1;
            }

            if((child_pid[i] = fork()) == 0){
                ep = 0, pid = i;

                // printf("enter child process[%d]\n\n", getpid());

                sigemptyset(&usr1_mask);
                sigaddset(&usr1_mask, SIGUSR1); 

                for(; ep < epoch; ++ep){
                    // printf("before process[%d] entering sigsuspend\n\n", getpid());

                    sigsuspend(&usr1_mask); //only block SIGUSR1

                    // printf("after process[%d] leaving sigsuspend\n\n", getpid());
                }
                
                exit(0);
            }
            
        }
        sigaddset(&blockall_mask, SIGUSR1);
        sigprocmask(SIG_SETMASK, &blockall_mask, NULL);
        
        act_p1.sa_handler = addDone;
        act_p1.sa_mask = blockall_mask;
        sigaction(SIGUSR1, &act_p1, NULL);

        act_p2.sa_handler = addDone;
        act_p2.sa_mask = blockall_mask;
        sigaction(SIGUSR2, &act_p2, NULL);

        for(int e = 0; e < epoch; ++e){
            if(e){
                while(done < q){
                    sigsuspend(&nonblock_mask);
                }
            }
            done = 0;
            for(int i = 0; i < q; ++i){

                // printf("kill child process[%d] SIGUSR2\n\n", child_pid[i]);

                if(kill(child_pid[i], SIGUSR2) < 0){
                    fprintf(stderr, "kill error\n");
                }
            }
        }

        // printf("iteration completed.\n");
        pid_t status;
        while(wait(&status) > 0);

        // chmod(outfile, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);
        write(output_fd, board[epoch%2], sizeof(char) * (n * (m+1)-1));
        
        munmap(board[0], sizeof(char) * n * (m+1));
        munmap(board[1], sizeof(char) * n * (m+1));
    }
    return 0;
}