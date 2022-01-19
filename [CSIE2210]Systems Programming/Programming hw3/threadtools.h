#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

extern int timeslice, switchmode;

typedef struct TCB_NODE *TCB_ptr;
typedef struct TCB_NODE{
    jmp_buf  Environment;
    int      Thread_id;
    TCB_ptr  Next;
    TCB_ptr  Prev;
    int i, N;
    int w, x, y, z;
} TCB;

extern jmp_buf MAIN, SCHEDULER;
extern TCB_ptr Head;
extern TCB_ptr Current;
extern TCB_ptr Work;
extern sigset_t base_mask, waiting_mask, tstp_mask, alrm_mask;

void sighandler(int signo);
void scheduler();

// Call function in the argument that is passed in
#define ThreadCreate(function, thread_id, init)        \
{                                                      \
    if(setjmp(MAIN) == 0)                              \
        function(thread_id, init);                     \
}

// Build up TCB_NODE for each function, insert it into circular linked-list
#define ThreadInit(thread_id, number)                   \
{                                                       \
	/* Please fill this code section. */	  			\
    if(!Head){                                          \
        Work = (TCB_ptr)malloc(sizeof(TCB));            \
        Work->Next = Work;                              \
        Work->Prev = Work;                              \
        Work->N = number;                               \
        Work->Thread_id = thread_id;                    \
        Head = Work;                                    \
        Current = Work;                                 \
    }                                                   \
    else{                                               \
        Current->Next = (TCB_ptr)malloc(sizeof(TCB));   \
        Current->Next->N = number;                      \
        Current->Next->Thread_id = thread_id;           \
        Current->Next->Next = Head;                     \
        Current->Next->Prev = Current;                  \
        Head->Prev = Current->Next;                     \
        Current = Current->Next;                        \
    }                                                   \
    if(setjmp(Current->Environment) == 0){              \
        longjmp(MAIN, 1);                               \
    }                                                   \
}

// Call this while a thread is terminated
#define ThreadExit()                                                                      \
{                                                                                         \
	/* Please fill this code section. */												  \
    longjmp(SCHEDULER, 2);                                                                \
}

// Decided whether to "context switch" based on the switchmode argument passed in main.c
#define ThreadYield()                                                                     \
{                                                                                         \
	/* Please fill this code section. */												  \
    sigset_t pending_mask;                                      \
    sigemptyset(&pending_mask);                                 \
    sigpending(&pending_mask);                                  \
    int val = setjmp(Current->Environment);                     \
    if(!val){                                                       \
        if(!switchmode){                                            \
            longjmp(SCHEDULER, 1);                                  \
        }                                                           \
        else{                                                       \
            if(sigismember(&pending_mask, SIGTSTP)){                \
                sigprocmask(SIG_UNBLOCK, &tstp_mask, NULL);         \
            }                                                       \
            else if(sigismember(&pending_mask, SIGALRM)){           \
                sigprocmask(SIG_UNBLOCK, &alrm_mask, NULL);         \
            }                                                       \
        }                                                           \
    }                                                               \
}
