#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 1


// threads entry point
void threads_entry(void * str, void * delay, void * not_used) {
    while(true) {
        printf( "%s", (char*)str );
        k_msleep( (int)delay );
    }
}

// create second thread using macro
K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
    (void*) "Thread 2\n",    /* argument 1         */ 
    (void*) 1200,            /* argument 2         */
    NULL,                    /* argument 3         */
    THREAD_PRIORITY,         /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

// statically define thread stack 
K_THREAD_STACK_DEFINE(thread_1_stack, THREAD_STACK_SIZE);

// thread control blocks
struct k_thread thread_1;

// thread ID
k_tid_t tid_1;


int main(void) {

    // create first thread dynamically
    tid_1 = k_thread_create(
        &thread_1,                              // thread control block 
        thread_1_stack,                         // thread stack 
        K_THREAD_STACK_SIZEOF(thread_1_stack),  // stack size
        threads_entry,                          // thread entry point
        (void*) "Thread 1\n",                   // argument 1
        (void*) 700,                            // argument 2 
        NULL,                                   // argument 3
        THREAD_PRIORITY,                        // thread priority
        0,                                      // thread options
        K_NO_WAIT                               // delay before thread start
    );


	return 0;
}
