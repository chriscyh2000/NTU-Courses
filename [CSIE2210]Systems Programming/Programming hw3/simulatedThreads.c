#include "threadtools.h"

// Please complete this three functions. You may refer to the macro function defined in "threadtools.h"

// Mountain Climbing
// You are required to solve this function via iterative method, instead of recursion.
void MountainClimbing(int thread_id, int number){
	/* Please fill this code section. */
	ThreadInit(thread_id, number);
	Current->x = 1;
	Current->y = 1;
	if(Current->N <= 1){
		sleep(1);
		printf("Mountain Climbing: 1\n");
		ThreadYield();
	} 
	for(Current->i = 2; Current->i <= Current->N; ++Current->i){
		sleep(1);
		printf("Mountain Climbing: %d\n", Current->x + Current->y);
		Current->z = Current->x + Current->y;
		Current->x = Current->y;
		Current->y = Current->z;
		ThreadYield();
	}
	ThreadExit();
}

// Reduce Integer
// You are required to solve this function via iterative method, instead of recursion.
void ReduceInteger(int thread_id, int number){
	/* Please fill this code section. */
	ThreadInit(thread_id, number);
	Current->i = 0;
	if(Current->N == 1){
		sleep(1);
		printf("Reduce Integer: 0\n");
		ThreadYield();
	} 
	while(Current->N > 1){
		sleep(1);
		++Current->i;
		printf("Reduce Integer: %d\n", Current->i);
		if(Current->N % 2 == 0){
			Current->N /= 2;
		}
		else if(Current->N % 4 == 1 || Current->N == 3){
			--Current->N;
		}
		else ++Current->N;
		ThreadYield();
	}
	ThreadExit();
}

// Operation Count
// You are required to solve this function via iterative method, instead of recursion.
void OperationCount(int thread_id, int number){
	/* Please fill this code section. */
	ThreadInit(thread_id, number);
	Current->z = 322 + Current->N;
	Current->x = 0;
	Current->y = Current->N - 1;
	Current->i = 0;
	if(Current->N <= 1){
		sleep(1);
		printf("Operation Count: 0\n");
		ThreadYield();
	} 
	while(Current->x < Current->y){
		sleep(1);
		Current->i += Current->z - (2 * Current->x + 323);
		++Current->x;
		--Current->y;
		printf("Operation Count: %d\n", Current->i);
		ThreadYield();
	}
	ThreadExit();
}
