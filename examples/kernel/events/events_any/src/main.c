#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

K_EVENT_DEFINE(event);

void threads_entry_trigger(void * event_bit, void * delay, void * arg_3) {
    while(true) {
        printf("Trigger thread post %d event bit\n", (int)event_bit);
        k_event_post(&event, BIT( (int)event_bit ));
        k_msleep( (int)delay );
    }
}

// Thread wait any of event bits
void threads_entry_wait(void * index, void * arg_2, void * arg_3) {
    uint32_t  event_bits;

    while(true) {
        event_bits = k_event_wait(&event, 0b11, true, K_FOREVER);
        printf("Wait thread get event bits: %x\n", event_bits);
    }
}

// thread 1 trigger wait for events
K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_wait,      /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    1,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);


// threads 2 and 3 trigger events
K_THREAD_DEFINE( 
    tid_2,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_trigger,   /* thread entry point */
    0,                       /* event bit          */
    1000,                    /* delay              */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_3,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_trigger,   /* thread entry point */
    1,                       /* event bit          */
    1400,                    /* delay              */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);
