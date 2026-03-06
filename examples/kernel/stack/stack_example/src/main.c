#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 
#define K_STACK_SIZE 5

K_STACK_DEFINE(stack, K_STACK_SIZE);	

int stack_data[K_STACK_SIZE];

// stack_data_t is pactically uintptr_t
void thread_sender(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        for(int i = 0; i < K_STACK_SIZE; i++) {
            stack_data[i] = i;
            printf("Sender thread send data to stack: %d\n", stack_data[i]);
            k_stack_push(&stack, (stack_data_t)&stack_data[i]);
        }

        k_msleep( 3000 );
    }
}

// k_stack_pop is expect to get stack_data_t* as data parameter. So data is pointer to pointer
// if receive thread wait for stack data, then first data item from sender passed directly to this thread 
void thread_receiver(void * arg_1, void * arg_2, void * arg_3) {
    int* data;
    
    while(true) {
        k_stack_pop(&stack, (stack_data_t*)&data, K_FOREVER);
        printf("Receiver thread get data from stack: %d\n", *data);
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


