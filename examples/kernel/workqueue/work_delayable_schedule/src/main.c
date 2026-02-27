#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 2
#define WORKQUEUE_PRIORITY 1

// work item handlers
void work_handler_1(struct k_work *work) {
    printf("Work item 1\n");
}

void work_handler_2(struct k_work *work) {
    printf("Work item 2\n");
}

// define work items
K_WORK_DELAYABLE_DEFINE(work_1, work_handler_1);
K_WORK_DELAYABLE_DEFINE(work_2, work_handler_2);

// thread send delayable work item period of sending is less than
// work item delay. Schedule not update work dalay tile, so work will by done every second
void threads_entry_1(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        k_work_schedule(&work_1, K_MSEC(1000));
        k_msleep( 500 );
    }
}

// thread send delayable work item period of sending is more than
// work item delay. work will by done every seconds
void threads_entry_2(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        k_work_schedule(&work_2, K_MSEC(500));
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
    0                        /* delay before start */
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
    0                        /* delay before start */
);
