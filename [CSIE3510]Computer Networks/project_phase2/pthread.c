#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define THREAD_NUM 10
void *do_something(void *ptr);

pthread_t tid[THREAD_NUM];

int main(int argc, char *argv[]){
	for(int i = 0 ; i < THREAD_NUM; i++){
        int *input = (int*)malloc(sizeof(int));
        *input = i * 100;
		// create a new thread.
		// pthread_create (ptr to a pthread_id, NULL, function to run, pointer to  the parameter)
		pthread_create(&tid[i], NULL, &do_something, (void*)input);
	}
	
	for(int i = 0 ; i < THREAD_NUM; i++){
		// wait a thread to be finished
		// pthread_join(ptr to a pthread_id, NULL)
		pthread_join(tid[i], NULL);
        printf("[Index %d] join pthread id %ld\n", i, tid[i]);
	}
}

void *do_something(void *arg){
	pthread_t id = pthread_self();
    int input = *(int*)arg;
    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_equal(id, tid[i])){
            printf("[Index %d] pthread id %ld with input value %d\n", i, tid[i], input);
            break;
        }
    }
    pthread_exit((void*)NULL);
}
