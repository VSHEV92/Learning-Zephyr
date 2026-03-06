#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 
#define DATA_ITEMS_NUMBER 5

int data_items[DATA_ITEMS_NUMBER];

K_QUEUE_DEFINE(queue);	

void thread_sender(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        for(int i = 0; i < DATA_ITEMS_NUMBER; i++) {
            data_items[i] = i;
            k_queue_alloc_append(&queue, &data_items[i]);
        }

        printf("Sender thread send data\n");
        k_msleep( 3000 );
    }
}

void thread_receiver(void * arg_1, void * arg_2, void * arg_3) {
    
    while(true) {
        int* data_item = (int*) k_queue_get(&queue, K_FOREVER);
        printf("Receiver thread get data: %d\n", *data_item);
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
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


