#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

// mutex definitions
K_MUTEX_DEFINE(mutex);

// shared data
uint32_t cnt = 0;

void threads_entry(void * index, void * delay, void * arg_3) {
    while(true) {
        k_busy_wait( (int)delay * 1000 );
        k_mutex_lock(&mutex, K_FOREVER);
        cnt++;
        k_mutex_unlock(&mutex);
        printf("Thread %d increment counter: %u\n", (int)index, cnt );
    }
}

K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
    1,                       /* index              */
    1000,                    /* delay (ms)         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
    2,                       /* index              */
    1500,                    /* delay (ms)         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);
