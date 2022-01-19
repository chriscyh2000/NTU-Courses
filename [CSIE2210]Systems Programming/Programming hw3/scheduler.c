#include "threadtools.h"

/*
1) You should state the signal you received by: printf('TSTP signal caught!\n') or printf('ALRM signal caught!\n')
2) If you receive SIGALRM, you should reset alarm() by timeslice argument passed in ./main
3) You should longjmp(SCHEDULER,1) once you're done.
*/
void sighandler(int signo){
	/* Please fill this code section. */
	if(!switchmode) longjmp(SCHEDULER, 1);
	else{
		if(signo == SIGTSTP){
			sigprocmask(SIG_SETMASK, &base_mask, NULL);
			printf("TSTP signal caught!\n");
		}
		else if(signo == SIGALRM){
			sigprocmask(SIG_SETMASK, &base_mask, NULL);
			printf("ALRM signal caught!\n");
			alarm(timeslice);
		}
		longjmp(SCHEDULER, 1);
	}
}

/*
1) You are stronly adviced to make 
	setjmp(SCHEDULER) = 1 for ThreadYield() case
	setjmp(SCHEDULER) = 2 for ThreadExit() case
2) Please point the Current TCB_ptr to correct TCB_NODE
3) Please maintain the circular linked-list here
*/
void scheduler(){
	/* Please fill this code section. */
	int schState = setjmp(SCHEDULER);
	if(!schState){
		Current = Head;
		longjmp(Current->Environment, 1);
	}
	else if(schState == 1){
		Current = Current->Next;
		longjmp(Current->Environment, 1);
	}
	else{
		if(Current == Current->Next){
			free(Current);
			Head = NULL;
			Current = NULL;
			longjmp(MAIN, 1);
		}
		Current->Prev->Next = Current->Next;
		Current->Next->Prev = Current->Prev;
		TCB_ptr tmp = Current;
		Current = Current->Next;
		free(tmp);
		longjmp(Current->Environment, 1);
	}
}
