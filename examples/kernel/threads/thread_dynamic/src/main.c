#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 1

// thread's stacks
k_thread_stack_t* thread_1_stack;
k_thread_stack_t* thread_2_stack;

// thread's control blocks
struct k_thread thread_1, thread_2;

// thread's ID's
k_tid_t tid_1, tid_2;

// threads entry point
void threads_entry(void * str, void * delay, void * not_used) {
    while(true) {
        printf( "%s", (char*)str );
        k_msleep( (int)delay );
    }
}

int main(void) {

    // allocate memory for stacks (size, flags)
    thread_1_stack = k_thread_stack_alloc(THREAD_STACK_SIZE, 0);
    thread_2_stack = k_thread_stack_alloc(THREAD_STACK_SIZE, 0);

    // create first thread
    tid_1 = k_thread_create(
        &thread_1,                              // thread control block 
        thread_1_stack,                         // thread stack 
        THREAD_STACK_SIZE,                      // stack size
        threads_entry,                          // thread entry point
        (void*) "Thread 1\n",                   // argument 1
        (void*) 700,                            // argument 2 
        NULL,                                   // argument 3
        THREAD_PRIORITY,                        // thread priority
        0,                                      // thread options
        K_NO_WAIT                               // delay before thread start
    );

    // create second thread
    tid_2 = k_thread_create(
        &thread_2,                              // thread control block 
        thread_2_stack,                         // thread stack 
        THREAD_STACK_SIZE,                      // stack size
        threads_entry,                          // thread entry point
        (void*) "Thread 2\n",                   // argument 1
        (void*) 1500,                           // argument 2 
        NULL,                                   // argument 3
        THREAD_PRIORITY,                        // thread priority
        0,                                      // thread options
        K_NO_WAIT                               // delay before thread start
    );

    k_sleep(K_FOREVER);

	return 0;
}
