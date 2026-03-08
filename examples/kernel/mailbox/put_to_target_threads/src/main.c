#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

K_MBOX_DEFINE(mbox);	

void thread_sender(void * arg_1, void * arg_2, void * arg_3);
void thread_receiver(void * idx, void * arg_2, void * arg_3);


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
    2,                       /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_3,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    thread_receiver,         /* thread entry point */
    3,                       /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

// this thread never get data
K_THREAD_DEFINE( 
    tid_4,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    thread_receiver,         /* thread entry point */
    4,                       /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


void thread_sender(void * arg_1, void * arg_2, void * arg_3) {

    int data = 0;
    struct k_mbox_msg msg;

    msg.size = sizeof(int);

    while(true) {
        data++;
        msg.tx_data = &data;

        // send data to 2 and 3 thread one by one
        if (data % 2) {
            msg.tx_target_thread = tid_2;
        }
        else {
            msg.tx_target_thread = tid_3;
        }

        printf("Sender thread send data: %d\n", data);
        k_mbox_put(&mbox, &msg, K_FOREVER);

        k_msleep( 2333 );
    }
}

void thread_receiver(void * idx, void * arg_2, void * arg_3) {

    int data;
    struct k_mbox_msg msg;
    
    msg.size = sizeof(int);
    msg.rx_source_thread = K_ANY;

    while(true) {
        k_mbox_get(&mbox, &msg, &data, K_FOREVER);
        printf("Receiver thread %d get data: %d\n", (int)idx, data);
    }
}
