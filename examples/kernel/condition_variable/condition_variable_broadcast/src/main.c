#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

K_MUTEX_DEFINE(mutex);
K_CONDVAR_DEFINE(condvar);


void threads_entry_trigger(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        // lock condition event mutex
        k_mutex_lock(&mutex, K_FOREVER);

        // add send signal to all thread in queue 
        k_condvar_broadcast(&condvar);

        printf("Thread 1 trigger condition variable event\n");
        
        // unlock condition event mutex
        k_mutex_unlock(&mutex);

        k_msleep(2000);
    }
}

void threads_entry_wait(void * index, void * arg_2, void * arg_3) {
    while(true) {
        // lock condition event mutex
        k_mutex_lock(&mutex, K_FOREVER);

        // add itself to condition variable queue 
        k_condvar_wait(&condvar, &mutex, K_FOREVER);
        
        printf("Thread %d get condition variable event\n", (int)index);
        
        // unlock condition event mutex
        k_mutex_unlock(&mutex);
    }
}

// thread 1 trigger condition variable
K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_trigger,   /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


// threads 2 and 3 wiat for condition variable
K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_wait,      /* thread entry point */
    2,                       /* index              */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_3,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_wait,      /* thread entry point */
    3,                       /* index              */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);
