#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

void threads_entry(void * str, void * dummy_1, void * dummy_2) {
    
    while(true) {
        // print message 5 times, then yield, so other thread can run
        for (int i = 0; i < 5; i++) {
            k_busy_wait( 1000000 );
            printf( "%s \n", (char*)str );
        }
        printf( "Thread is yielding\n" );
        k_yield();
    }
}


K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
    (void*) "Thread 1",      /* argument 1         */ 
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
    (void*) "Thread 2",      /* argument 1         */ 
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

