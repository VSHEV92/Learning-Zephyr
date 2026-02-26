#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

void threads_1_entry(void * arg1, void * arg2, void * arg3);
void threads_2_entry(void * arg1, void * arg2, void * arg3);


// create susupened thread
K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_1_entry,         /* thread entry point */
    NULL,                    /* argument 1         */ 
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

// create resuming thread
K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_2_entry,         /* thread entry point */
    NULL,                    /* argument 1         */ 
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

void threads_1_entry(void * arg1, void * arg2, void * arg3) {
    while(true) {
        
        // print 5 strings and suspend
        for (int i = 0; i < 5; i++) {
            printf("Thread 1 rinning\n");
            k_busy_wait(200000);
        }

        // get current thread id
        printf("Thread 1 suspend itself\n");
        k_tid_t tid = k_current_get();
        k_thread_suspend(tid);
    }
}

void threads_2_entry(void * arg1, void * arg2, void * arg3) {
    while(true) {
        
        // resume thread 1 every 3 seconds
        printf("Thread 2 rinning\n");
        k_msleep(3000);

        printf("Thread 2 resumimg thread 1\n");
        k_thread_resume(tid_1);
    }
}
