#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0

#define NUM_CALL_THRD 40

void f1( void *k )
{
    int *kk = (int *)k;
    *kk = *(int *)k + 1;
    if( *kk <= NUM_CALL_THRD )
    {
        struct thread *t1 = thread_create(f1, k, 3);
        thread_add_runqueue(t1);
    }    
    thread_exit();
}

int main()
{
    int test2k = 0;
    struct thread *t1 = thread_create(f1, &test2k, 3);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("test2k==%d\n", test2k);
    if( test2k != NUM_CALL_THRD + 1 ){
        printf("FAILED. test2k should equal to %d\n", NUM_CALL_THRD + 1);
        exit(0);
    }
    printf("eac8a36c12c94b93e22928c1174372d15dca00a5724481788a7a0a901e065338 3\n", test2k);
    
    exit(0);
}
