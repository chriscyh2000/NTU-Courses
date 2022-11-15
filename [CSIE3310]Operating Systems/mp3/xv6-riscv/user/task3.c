#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
int k2 = 0;
int k3 = 0;
int k1 = 0;
void f3(void *arg)
{
    while (1)
    {
        k3 ++;
    }

    
}

void f2(void *arg)
{
    while(1) {
         k2 ++;
    }
}

void f1(void *arg)
{
    k1 = 0;
    while(1) {
            k1 += 1;
    }
}

int main(int argc, char **argv)
{
    struct thread *t1 = thread_create(f2, NULL, 4);
    thread_add_runqueue(t1);
    struct thread *t2 = thread_create(f1, NULL, 3);
    thread_add_runqueue(t2);

    struct thread *t3 = thread_create(f2, NULL, 3);
    thread_add_runqueue(t3);

    struct thread *t4 = thread_create(f2, NULL, 1);
    thread_add_at(t4, 5);

    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}