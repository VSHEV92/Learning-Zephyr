#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

K_MBOX_DEFINE(mbox);	

void thread_sender(void * arg_1, void * arg_2, void * arg_3) {

    int data = 0;
    struct k_mbox_msg msg;

    msg.size = sizeof(int);
    msg.tx_target_thread = K_ANY;

    while(true) {
        data++;
        msg.tx_data = &data;

        printf("Sender thread send data: %d\n", data);
        k_mbox_put(&mbox, &msg, K_FOREVER);
        printf("Sender thread get response from receiver\n");

        k_msleep( 2333 );
    }
}

void thread_receiver(void * arg_1, void * arg_2, void * arg_3) {

    int data;
    struct k_mbox_msg msg;
    
    msg.size = sizeof(int);
    msg.rx_source_thread = K_ANY;

    while(true) {
        k_mbox_get(&mbox, &msg, NULL, K_FOREVER);
        printf("Receive thread get message\n");

        k_msleep( 1500 );

        k_mbox_data_get(&msg, &data);
        printf("Receive thread retreive data: %d\n", data);
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
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


