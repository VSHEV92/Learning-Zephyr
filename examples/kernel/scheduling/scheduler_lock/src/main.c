#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

void threads_entry_1(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        // print message 5 time, then lock scheduler
        for (int i = 0; i < 5; i++) {
            printf( "Thread 1\n" );
            k_busy_wait( 500 * 1000 );
        }

        k_sched_lock();
        printf( "Thread 1 locked scheduler. Only this thread will run\n" );


        // print message 5 time, then lock scheduler
        for (int i = 0; i < 5; i++) {
            printf( "Thread 1\n" );
            k_busy_wait( 500 * 1000);
        }

        printf( "Thread 1 will unlock scheduler. Other threads can run\n" );
        k_sched_unlock();
    }
}

void threads_entry_2(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        printf( "Thread 2\n" );
        k_busy_wait( 500 * 1000 );

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
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

