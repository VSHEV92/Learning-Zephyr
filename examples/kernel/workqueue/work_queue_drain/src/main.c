#include <stdio.h>
#include <zephyr/kernel.h>

// stack size in bytes
#define THREAD_STACK_SIZE 1024 

// define threads priority
#define THREAD_PRIORITY 1

// work item handlers
void work_handler(struct k_work *work) {
    printf("Work submited\n");
    k_msleep( 1000 );
    printf("Work item done\n");
}

// define work items
K_WORK_DEFINE(work_item_1, work_handler);
K_WORK_DEFINE(work_item_2, work_handler);
K_WORK_DEFINE(work_item_3, work_handler);

// threads 1 send work item to queue
void threads_entry_1(void * arg_1, void * arg_2, void * arg_3) {
    // submit 3 items to queue and sleep
    k_work_submit(&work_item_1);
    k_work_submit(&work_item_2);
    k_work_submit(&work_item_3);
}

// thread 2 always wait when work item is flushed
void threads_entry_2(void * arg_1, void * arg_2, void * arg_3) {
    printf("Thread 2 wait workqueue drain\n");
    k_work_queue_drain(&k_sys_work_q, false);
    printf("Thread 2 waiting complete\n");
}

// start work items submit
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

// thread have highest priorty. when work item flushed, it is unblocked
K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_2,         /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    0,                       /* thread priority    */ 
    0,                       /* thread options     */
    1                        /* delay before start */
);
