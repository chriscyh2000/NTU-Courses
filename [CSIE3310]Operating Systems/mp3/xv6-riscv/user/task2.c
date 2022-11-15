#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
int k = 0;
int k1 = 0;
void f3(void *arg)
{
    while (1)
    {
        k ++;
    }

    
}

void f2(void *arg)
{
    while(1) {
         k ++;
    }
}

void f1(void *arg)
{
    int start_time = uptime();
    k1 = 0;
    while(1) {
         if(k1 == 0 && uptime() - start_time >= 2)
         {
            k1 = 1;
            thread_yield();
         }
    }
}

int main(int argc, char **argv)
{
    struct thread *t1 = thread_create(f1, NULL, 3);
    thread_add_runqueue(t1);
    struct thread *t2 = thread_create(f2, NULL, 3);
    thread_add_runqueue(t2);

    struct thread *t3 = thread_create(f3, NULL, 3);
    thread_add_at(t3, 7);

    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}