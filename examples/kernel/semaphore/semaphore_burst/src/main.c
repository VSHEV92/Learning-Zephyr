#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 
#define SEMAPHORE_LIMIT 5

// semaphore definitions
K_SEM_DEFINE(
    sem,                /* semaphore name */ 
    0,                  /* initial count */
    SEMAPHORE_LIMIT     /* limit */
);

// thread 1 have highger priority and give semaphore up to linit
void threads_entry_give(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        for (int i = 0; i < SEMAPHORE_LIMIT; i++) {
            printf("Thread 1 give semaphore\n");
            k_sem_give(&sem);
        }
        k_msleep( 5000 );
    }
}

// threads 2 and 3 have lower priority and take semaphore
void threads_entry_take(void * idx, void * delay, void * arg_3) {
    while(true) {
        k_sem_take(&sem, K_FOREVER);
        printf("Thread %d take semaphore\n", (int)idx);
        k_msleep( (int)delay );
    }
}

K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_give,      /* thread entry point */
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
    threads_entry_take,      /* thread entry point */
    2,                       /* index              */
    600,                     /* delay              */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_3,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_take,      /* thread entry point */
    3,                       /* index              */
    1000,                    /* delay              */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);
