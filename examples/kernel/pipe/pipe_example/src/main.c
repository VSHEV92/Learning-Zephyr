#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

#define PIPE_BUFFER_SIZE 5
#define PIPE_BUFFER_ALIGN 4

#define SEND_DATA_BUFFER_SIZE (PIPE_BUFFER_SIZE + 3) 

K_PIPE_DEFINE(pipe, PIPE_BUFFER_SIZE, PIPE_BUFFER_ALIGN);	

void thread_sender(void * arg_1, void * arg_2, void * arg_3) {

    uint8_t data[SEND_DATA_BUFFER_SIZE];

    for (int i = 0; i < SEND_DATA_BUFFER_SIZE; i++) {
        data[i] = i;
    }

    while(true) {

        printf("Sender thread start sending data\n");
        k_pipe_write(&pipe, data, SEND_DATA_BUFFER_SIZE, K_FOREVER);
        printf("Sender thread finish sending data\n");

        k_msleep( 5000 );
    }
}

void thread_receiver(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        uint8_t data;
        k_pipe_read(&pipe, &data, 1, K_FOREVER);
        printf("Receiver thread get data: %d\n", data);
        k_msleep( 500 );
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


