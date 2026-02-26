#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 1


// statically define thread stack 
K_THREAD_STACK_DEFINE(thread_1_stack, THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_2_stack, THREAD_STACK_SIZE);

// thread control blocks
struct k_thread thread_1, thread_2;

// thread ID
k_tid_t tid_1, tid_2;


void threads_entry_1(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        
        // try to sleep 1 second
        int status = k_msleep( 1000 );
        
        if (status) {
            printf( "Thread 1 was waked up\n" );
        }
        else {
            printf( "Thread 1 sleep all it's time\n" );
        }
    }
}


void threads_entry_2(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        
        // try to sleep 5 seconds, then wakeup other thread
        k_msleep( 4300 );
        printf( "Thread 2 wake up other thread\n" );
        k_wakeup(tid_1);
    }
}


int main(void) {

    tid_1 = k_thread_create(
        &thread_1,                              // thread control block 
        thread_1_stack,                         // thread stack 
        K_THREAD_STACK_SIZEOF(thread_1_stack),  // stack size
        threads_entry_1,                        // thread entry point
        NULL,                                   // argument 1
        NULL,                                   // argument 2 
        NULL,                                   // argument 3
        THREAD_PRIORITY,                        // thread priority
        0,                                      // thread options
        K_NO_WAIT                               // delay before thread start
    );

    tid_2 = k_thread_create(
        &thread_2,                              // thread control block 
        thread_2_stack,                         // thread stack 
        K_THREAD_STACK_SIZEOF(thread_2_stack),  // stack size
        threads_entry_2,                        // thread entry point
        NULL,                                   // argument 1
        NULL,                                   // argument 2 
        NULL,                                   // argument 3
        THREAD_PRIORITY,                        // thread priority
        0,                                      // thread options
        K_NO_WAIT                               // delay before thread start
    );

	return 0;
}
