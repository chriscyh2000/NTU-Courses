#include "threadtools.h"

jmp_buf MAIN;
jmp_buf SCHEDULER;
TCB_ptr Head;
TCB_ptr Current;
TCB_ptr Work;
struct sigaction act;
sigset_t base_mask, waiting_mask, tstp_mask, alrm_mask;
int mainstate, timeslice, switchmode;

/* prototype of three functions */
void ReduceInteger(int thread_id, int number);
void MountainClimbing(int thread_id, int number);
void OperationCount(int thread_id, int number);

int main(int argc, char *argv[]){
    int ri_int = atoi(argv[1]);
    int mc_heights = atoi(argv[2]);
    int oc_arrlen = atoi(argv[3]);
    timeslice = atoi(argv[4]);
    switchmode = atoi(argv[5]);

    // set signal masks
    sigemptyset(&base_mask); // initialize base_mask signal set
    sigaddset(&base_mask, SIGTSTP); // add signal: SIGTSTP to base_mask
    sigaddset(&base_mask, SIGALRM); // add signal: SIGALRM to base_mask
    sigemptyset(&tstp_mask); // initialize tstp_mask signal set
    sigemptyset(&alrm_mask); // initialize alrm_mask signal set
    sigaddset(&tstp_mask, SIGTSTP); // add signal: SIGTSTP to tstp_mask
    sigaddset(&alrm_mask, SIGALRM); // add signal: SIGALRM to alrm_mask
        
    act.sa_handler = sighandler;
    sigaction(SIGTSTP, &act, NULL); 
    sigaction(SIGALRM, &act, NULL);
    sigprocmask(SIG_SETMASK, &base_mask, NULL);

    Head = Current = NULL;

    ThreadCreate(ReduceInteger, 1, ri_int);
    ThreadCreate(MountainClimbing, 2, mc_heights);
    ThreadCreate(OperationCount, 3, oc_arrlen);

    alarm(timeslice);
    mainstate = setjmp(MAIN);
    if( mainstate == 0 ) scheduler();
    return 0;
}
