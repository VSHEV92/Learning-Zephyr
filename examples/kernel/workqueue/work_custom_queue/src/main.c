#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 2
#define WORKQUEUE_PRIORITY 1

// custom workqueue
struct k_work_q custom_work_q;

// work item handlers
void work_handler_1(struct k_work *work) {
    printf("Work item 1\n");
}

void work_handler_2(struct k_work *work) {
    printf("Work item 2\n");
}

// define work items
struct k_work work_1, work_2;

// threads entry point
void threads_entry_1(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        k_work_submit_to_queue(&custom_work_q, &work_1);
        k_msleep( 700 );
    }
}

void threads_entry_2(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        k_work_submit_to_queue(&custom_work_q, &work_2);
        k_msleep( 1000 );
    }
}

// create threads using macro
K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_1,         /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    THREAD_PRIORITY,         /* thread priority    */ 
    0,                       /* thread options     */
    1                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_2,         /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    THREAD_PRIORITY,         /* thread priority    */ 
    0,                       /* thread options     */
    1                        /* delay before start */
);

// define stack for custom workqueue thread
K_THREAD_STACK_DEFINE(custom_queue_thread_stack, THREAD_STACK_SIZE);

int main() {

    // create work items
    k_work_init(&work_1, work_handler_1);
    k_work_init(&work_2, work_handler_2);

    // init and start custom workqueue
    k_work_queue_init(&custom_work_q);

    k_work_queue_start(
        &custom_work_q,             // workqueue
        custom_queue_thread_stack,  // workqueue thread stack
        THREAD_STACK_SIZE,          // workqueue thread size
        THREAD_PRIORITY - 1,        // workqueue thread priority
        NULL                        // workqueue config struct (name, no_yield, essential, timeout)
    );

    return 0;
}
