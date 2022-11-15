#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0

// TODO: necessary declares, if any
static struct thread* current_thread = NULL;
static struct thread* root_thread = NULL;
static int id = 1;
int top = 0;
static struct thread* t_stack[64];
static jmp_buf env_st;
static jmp_buf env_tmp;


struct thread *thread_create(void (*f)(void *), void *arg){
    struct thread *t = (struct thread*) malloc(sizeof(struct thread));
    //unsigned long stack_p = 0;
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack   = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack +0x100*8-0x2*8;
    t->fp      = f;
    t->arg     = arg;
    t->ID      = id;
    t->buf_set = 0;
    t->stack   = (void*) new_stack;
    t->stack_p = (void*) new_stack_p;
    id++;
    return t;
}
void thread_add_runqueue(struct thread *t){
    t->left   = NULL;
    t->right  = NULL;
    t->parent = NULL;
    if(current_thread == NULL){
        // TODO
        current_thread = t;
        root_thread    = t;
    }
    else{
        // TODO
        t->parent = current_thread;
        if(current_thread->left == NULL){
            current_thread->left = t;
            t_stack[top++] = t;
        }
        else if(current_thread->right == NULL){
            current_thread->right = t;
            t_stack[top] = t_stack[top-1];
            t_stack[top-1] = t;
            top++;
        }
        else free(t);
    }
}
void thread_yield(void){
    // TODO
    int val;
    if((val = setjmp(current_thread->env)) == 0){
        current_thread->buf_set = 1;
        schedule();
        dispatch();
    }
}
void dispatch(void){
    // TODO
    if(current_thread->buf_set == 0){
        int val;
        if((val = setjmp(env_tmp)) == 0){
            env_tmp->sp = (unsigned long)current_thread->stack_p;
            longjmp(env_tmp, 1);
        }
        else{
            current_thread->fp(current_thread->arg);
            thread_exit();
        }
    }
    else longjmp(current_thread->env, 1);
}
void schedule(void){
    // TODO    
    current_thread = t_stack[--top];
    if(current_thread->right != NULL){
        t_stack[top++] = current_thread->right;
    }
    if(current_thread->left != NULL){
        t_stack[top++] = current_thread->left;
    }
    if(top == 0){
       t_stack[top++] = root_thread; 
    } 
}
void thread_exit(void){
    if(current_thread == root_thread && current_thread->left == NULL && current_thread->right == NULL){
        // TODO
        // Hint: No more thread to execute
        root_thread = NULL;
        free(current_thread->stack);
        free(current_thread);
        current_thread = NULL;
        longjmp(env_st, 1);
    }
    else{
        // TODO
        struct thread *cur = current_thread;
        while(cur->left != NULL || cur->right != NULL){
            cur = (cur->right != NULL) ? cur->right : cur->left;
        }

        if(current_thread == root_thread){
            if(t_stack[0] == root_thread){
                for(int i = 0; i <= top-2; ++i){
                    t_stack[i] = t_stack[i+1];
                }
                top--;
            }
            root_thread = cur;
        }
        if(cur == current_thread->left){
            top--;
            if(top == 0) t_stack[top++] = root_thread;
        }
        else if(cur == current_thread->right){
            if(current_thread->left == NULL){
                top--;
                if(top == 0) t_stack[top++] = root_thread;
            }
            else{
                t_stack[top-2] = t_stack[top-1];
                top--;
            }
        }
        if(cur->parent->left == cur){ // left child
            cur->parent->left = NULL;
        } 
        else{ // right child
            cur->parent->right = NULL;
        }

        cur->left = current_thread->left;
        cur->right = current_thread->right;
        cur->parent = current_thread->parent;

        if(cur->left != NULL){
            cur->left->parent = cur;
        }
        if(cur->right != NULL){
            cur->right->parent = cur;
        }
        if(current_thread->parent != NULL && cur != current_thread){
            if(current_thread->parent->left == current_thread){
                current_thread->parent->left = cur;
            }
            else current_thread->parent->right = cur;
        }
        free(current_thread->stack);
        free(current_thread);
        schedule();
        dispatch();
    }
}
void thread_start_threading(void){
    // TODO
    int val;
    if((val = setjmp(env_st)) == 0){
        dispatch();
    }
}