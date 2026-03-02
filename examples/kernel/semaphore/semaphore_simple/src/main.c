#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

// semaphore definitions
K_SEM_DEFINE(
    sem,  /* semaphore name */ 
    0,    /* initial count */
    1     /* limit */
);

// thread 1 have highger priority and wait semaphore
void threads_entry_1(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        printf("Thread 1 wait semaphore\n");
        k_sem_take(&sem, K_FOREVER);
        printf("Thread 1 take semaphore\n");
        k_msleep( 500 );
    }
}

// thread 2 have lower priority and give semaphore
void threads_entry_2(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        printf("Thread 2 give semaphore\n");
        k_sem_give(&sem);
        k_msleep( 2000 );
    }
}

K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_1,         /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_2,         /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);
