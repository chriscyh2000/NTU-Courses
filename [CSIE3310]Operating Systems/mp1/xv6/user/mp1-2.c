#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0

void f15(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 15: %d\n", i++);
        if (i == 17) {
            thread_exit();
        }
        thread_yield();
    }
}

void f14(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 14: %d\n", i++);
        if (i == 16) {
            thread_exit();
        }
        thread_yield();
    }
}

void f13(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 13: %d\n", i++);
        if (i == 15) {
            thread_exit();
        }
        thread_yield();
    }
}

void f12(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 12: %d\n", i++);
        if (i == 14) {
            thread_exit();
        }
        thread_yield();
    }
}

void f11(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 11: %d\n", i++);
        if (i == 13) {
            thread_exit();
        }
        thread_yield();
    }
}

void f10(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 10: %d\n", i++);
        if (i == 12) {
            thread_exit();
        }
        thread_yield();
    }
}

void f9(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 9: %d\n", i++);
        if (i == 11) {
            thread_exit();
        }
        thread_yield();
    }
}

void f8(void *arg)
{
    int i = 0;
    while(1) {
        printf("thread 8: %d\n", i++);
        if (i == 10) {
            thread_exit();
        }
        thread_yield();
    }
}

void f7(void *arg)
{
    int i = 0;

    struct thread *t14 = thread_create(f14, NULL);
    thread_add_runqueue(t14);
    struct thread *t15 = thread_create(f15, NULL);
    thread_add_runqueue(t15);

    while(1) {
        printf("thread 7: %d\n", i++);
        if (i == 9) {
            thread_exit();
        }
        thread_yield();
    }
}

void f6(void *arg)
{
    int i = 0;

    struct thread *t12 = thread_create(f12, NULL);
    thread_add_runqueue(t12);
    struct thread *t13 = thread_create(f13, NULL);
    thread_add_runqueue(t13);

    while(1) {
        printf("thread 6: %d\n", i++);
        if (i == 8) {
            thread_exit();
        }
        thread_yield();
    }
}

void f5(void *arg)
{
    int i = 0;

    struct thread *t10 = thread_create(f10, NULL);
    thread_add_runqueue(t10);
    struct thread *t11 = thread_create(f11, NULL);
    thread_add_runqueue(t11);

    while(1) {
        printf("thread 5: %d\n", i++);
        if (i == 7) {
            thread_exit();
        }
        thread_yield();
    }
}

void f4(void *arg)
{
    int i = 0;

    struct thread *t8 = thread_create(f8, NULL);
    thread_add_runqueue(t8);
    struct thread *t9 = thread_create(f9, NULL);
    thread_add_runqueue(t9);

    while(1) {
        printf("thread 4: %d\n", i++);
        if (i == 6) {
            thread_exit();
        }
        thread_yield();
    }
}

void f3(void *arg)
{
    int i = 0;

    struct thread *t6 = thread_create(f6, NULL);
    thread_add_runqueue(t6);
    struct thread *t7 = thread_create(f7, NULL);
    thread_add_runqueue(t7);

    while(1) {
        printf("thread 3: %d\n", i++);
        if (i == 5) {
            thread_exit();
        }
        thread_yield();
    }
}

void f2(void *arg)
{
    int i = 0;

    struct thread *t4 = thread_create(f4, NULL);
    thread_add_runqueue(t4);
    struct thread *t5 = thread_create(f5, NULL);
    thread_add_runqueue(t5);

    while(1) {
        printf("thread 2: %d\n", i++);
        if (i == 4) {
            thread_exit();
        }
        thread_yield();
    }
}

void f1(void *arg)
{
    int i = 0;

    struct thread *t2 = thread_create(f2, NULL);
    thread_add_runqueue(t2);
    struct thread *t3 = thread_create(f3, NULL);
    thread_add_runqueue(t3);
    
    while(1) {
        printf("thread 1: %d\n", i++);
        if (i == 3) {
            thread_exit();
        }
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    printf("mp1-2\n");
    struct thread *t1 = thread_create(f1, NULL);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}
