#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 1


// threads entry point, get and print itself name
void threads_entry(void * arg1, void * arg2, void * arg3) {

    k_tid_t tid = k_current_get();
    const char* current_thread_name = k_thread_name_get(tid); 
    
    while(true) {
        printf( "Thread %s is running\n", (char*)current_thread_name );
        k_msleep( 1000 );
    }
}

// statically define thread stack 
K_THREAD_STACK_DEFINE(thread_1_stack, THREAD_STACK_SIZE);

// thread control blocks
struct k_thread thread_1;

// thread ID
k_tid_t tid_1;

// thread name
char thread_name[] = "dummy_thread";

int main(void) {

    // create first thread dynamically and delay it start forever
    tid_1 = k_thread_create(
        &thread_1,                              // thread control block 
        thread_1_stack,                         // thread stack 
        K_THREAD_STACK_SIZEOF(thread_1_stack),  // stack size
        threads_entry,                          // thread entry point
        NULL,                                   // argument 1
        NULL,                                   // argument 2 
        NULL,                                   // argument 3
        THREAD_PRIORITY,                        // thread priority
        0,                                      // thread options
        K_FOREVER                               // delay before thread start
    );

    // set thread name
    k_thread_name_set(tid_1, thread_name);

    // start thread
    k_thread_start(tid_1);	

	return 0;
}
