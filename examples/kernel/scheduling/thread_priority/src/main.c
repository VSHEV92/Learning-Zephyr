#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

void threads_entry_1(void * str, void * delay, void * not_used) {
    k_tid_t tid = k_current_get();
    int priority = k_thread_priority_get(tid);
    
    while(true) {
        
        // print message 5 time, that set itself priority lower, so other thread can run
        for (int i = 0; i < 5; i++) {
            printf( "%s, priority %d\n", (char*)str, priority );
            k_busy_wait( (int)delay * 1000 );
        }

        printf( "%s, set priority lower, other thread can run\n", (char*)str );
        priority += 2;
        k_thread_priority_set(tid, priority);


        // print message 5 time, that set itself priority higher, so only this thread will run
        for (int i = 0; i < 5; i++) {
            printf( "%s, priority %d\n", (char*)str, priority );
            k_busy_wait( (int)delay * 1000 );
        }

        printf( "%s, set priority highter, only this thread will run\n", (char*)str );
        priority -= 2;
        k_thread_priority_set(tid, priority);

    }
}


void threads_entry_2(void * str, void * delay, void * not_used) {
    k_tid_t tid = k_current_get();
    int priority = k_thread_priority_get(tid);

    while(true) {
        // print message and sleep, so other thread can run 
        printf( "%s, priority %d\n", (char*)str, priority );
        k_msleep( (int)delay );
    }
}


K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_1,         /* thread entry point */
    (void*) "Thread 1",      /* argument 1         */ 
    (void*) 500,             /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_2,         /* thread entry point */
    (void*) "Thread 2",      /* argument 1         */ 
    (void*) 200,             /* argument 2         */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

