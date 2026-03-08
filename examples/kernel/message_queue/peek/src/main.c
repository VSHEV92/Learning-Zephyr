#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 
#define DATA_ITEMS_NUMBER 5

K_MSGQ_DEFINE(msgq, sizeof(int), DATA_ITEMS_NUMBER, 1);	

void thread_sender(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        for(int i = 0; i < DATA_ITEMS_NUMBER; i++) {
            k_msgq_put(&msgq, &i, K_FOREVER);
            printk("Sender thread send data: %d\n", i);
        }
        k_msleep( 7000 );
    }
}

void thread_peeker(void * arg_1, void * arg_2, void * arg_3) {
    
    while(true) {
        int data, rc;
        rc = k_msgq_peek(&msgq, &data);
        if (rc) {
            printk("Peeker thread not get data\n");
        }
        else {
            printk("Peeker thread get data: %d\n", data);
        }
        k_msleep( 1000 );
    }
}

void thread_receiver(void * arg_1, void * arg_2, void * arg_3) {
    
    while(true) {
        int data;
        k_msgq_get(&msgq, &data, K_FOREVER);
        printk("Receiver thread get data: %d\n", data);
        k_msleep( 1366 );
    }
}

K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    thread_sender,           /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    thread_receiver,         /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    3,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_3,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    thread_peeker,           /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);
