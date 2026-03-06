#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 
#define DATA_ITEMS_NUMBER 5


struct data_item_t {
    void *next;
    int data;
};

struct data_item_t data_items[DATA_ITEMS_NUMBER];

K_QUEUE_DEFINE(queue);	

void thread_sender(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        for(int i = 0; i < DATA_ITEMS_NUMBER-1; i++) {
            data_items[i].data = i;
            data_items[i].next = &data_items[i+1];
        }

        data_items[DATA_ITEMS_NUMBER-1].data = DATA_ITEMS_NUMBER-1;
        data_items[DATA_ITEMS_NUMBER-1].next = NULL;

        k_queue_append_list(&queue, &data_items[0], &data_items[DATA_ITEMS_NUMBER-1]);

        printf("Sender thread send data\n");
        k_msleep( 3000 );
    }
}

void thread_receiver(void * arg_1, void * arg_2, void * arg_3) {
    struct data_item_t* data_item;
    
    while(true) {
        data_item = (struct data_item_t*) k_queue_get(&queue, K_FOREVER);
        printf("Receiver thread get data: %d\n", data_item->data);
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


