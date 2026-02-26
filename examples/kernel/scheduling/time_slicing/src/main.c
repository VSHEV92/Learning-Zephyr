#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

void threads_entry(void * str, void * delay, void * not_used) {
    k_tid_t tid = k_current_get();
    int priority = k_thread_priority_get(tid);
    
    while(true) {
        // print message 5 time, that set itself priority to time slicing zone
        for (int i = 0; i < 5; i++) {
            printf( "%s, priority %d\n", (char*)str, priority );
            k_busy_wait( (int)delay * 1000 );
        }

        printf( "%s, set priority lower, thread in time slicing zone\n", (char*)str );
        k_thread_priority_set(tid, ++priority);


        // print message 5 time, that set itself priority higher, so thread is not time sliced
        for (int i = 0; i < 5; i++) {
            printf( "%s, priority %d\n", (char*)str, priority );
            k_busy_wait( (int)delay * 1000 );
        }

        printf( "%s, set priority highter, only this thread will run\n", (char*)str );
        k_thread_priority_set(tid, --priority);
    }
}


K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
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
    threads_entry,           /* thread entry point */
    (void*) "Thread 2",      /* argument 1         */ 
    (void*) 500,             /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

