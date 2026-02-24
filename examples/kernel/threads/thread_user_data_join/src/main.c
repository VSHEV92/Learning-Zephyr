#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 1

// threads entry point
void threads_entry(void * str, void * delay, void * cnt) {
    
    // init custom data to zero
    k_thread_custom_data_set(0);

    while(true) {
        // get custom data
        int current_cnt = (int)k_thread_custom_data_get();

        // update custom data
        k_thread_custom_data_set( (void*)(++current_cnt) );
        
        printf( "%s %0d\n", (char*)str, current_cnt );
        k_msleep( (int)delay );

        if ((int)cnt == current_cnt)
            break;
    }

}

int main(void) {

    printf("Main: Allocate stack and start threads\n");

    // allocate stacks
    k_thread_stack_t* thread_stack_1 = k_thread_stack_alloc(THREAD_STACK_SIZE, 0);
    k_thread_stack_t* thread_stack_2 = k_thread_stack_alloc(THREAD_STACK_SIZE, 0);

    // create threads
    struct k_thread thread_1, thread_2;
    k_thread_create(
        &thread_1,                            // thread control block 
        thread_stack_1,                       // thread stack 
        THREAD_STACK_SIZE,                    // stack size
        threads_entry,                        // thread entry point
        (void*) "Thread 1 running",           // argument 1
        (void*) 700,                          // argument 2 
        (void*) 5,                            // argument 3
        THREAD_PRIORITY,                      // thread priority
        0,                                    // thread options
        K_NO_WAIT                             // delay before thread start
    );

    k_tid_t tid_2 = k_thread_create(
        &thread_2,                            // thread control block 
        thread_stack_2,                       // thread stack 
        THREAD_STACK_SIZE,                    // stack size
        threads_entry,                        // thread entry point
        (void*) "Thread 2 running",           // argument 1
        (void*) 500,                          // argument 2 
        (void*) 100,                          // argument 3
        THREAD_PRIORITY,                      // thread priority
        0,                                    // thread options
        K_NO_WAIT                             // delay before thread start
    );
    
    // join to thread
    printf("Main: Join to thread 1\n");
    k_thread_join(&thread_1, K_FOREVER);

    // abort thread 2
    printf("Main: Abort thread 2\n");
    k_thread_abort(tid_2);

    // free stack
    printf("Main: Free stacks\n");
    k_thread_stack_free(thread_stack_1);
    k_thread_stack_free(thread_stack_2);

	return 0;
}
