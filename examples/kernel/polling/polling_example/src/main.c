#include <stdio.h>
#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024 

K_SEM_DEFINE(sem_1, 0, 1);
K_SEM_DEFINE(sem_2, 0, 1);

struct k_poll_signal poll_signal;

struct k_poll_event events[] = {
    K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_SEM_AVAILABLE,
                                    K_POLL_MODE_NOTIFY_ONLY,
                                    &sem_1, 0),
    K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_SEM_AVAILABLE,
                                    K_POLL_MODE_NOTIFY_ONLY,
                                    &sem_2, 0),
    K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_SIGNAL,
                                    K_POLL_MODE_NOTIFY_ONLY,
                                    &poll_signal, 0)
};

void threads_entry_polling(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        k_poll(events, ARRAY_SIZE(events), K_FOREVER);
        
        if (events[0].state == K_POLL_STATE_SEM_AVAILABLE) {
            k_sem_take(events[0].sem, K_NO_WAIT);
            printf("Notify from semaphore 1\n");
        }
        
        if (events[1].state == K_POLL_STATE_SEM_AVAILABLE) {
            k_sem_take(events[1].sem, K_NO_WAIT);
            printf("Notify from semaphore 2\n");
        }

        int signaled, result;
        k_poll_signal_check(events[2].signal, &signaled, &result);
        if (signaled) {
            printf("Notify from poll signal\n");
            k_poll_signal_reset(&poll_signal);
        }

        events[0].state = K_POLL_STATE_NOT_READY;
        events[1].state = K_POLL_STATE_NOT_READY;
        events[2].state = K_POLL_STATE_NOT_READY;
    }
}

void trigger_semaphore(void * sem, void * delay, void * arg_3) {
    while(true) {
        printf("Thread give semaphore\n");
        k_sem_give(sem);
        k_msleep( (int)delay );
    }
}

void trigger_signal(void * arg_1, void * arg_2, void * arg_3) {
    while(true) {
        printf("Thread trigger signal\n");
        k_poll_signal_raise(&poll_signal, 0);
        k_msleep( 2000 );
    }
}

// thread 1 polling events
K_THREAD_DEFINE( 
    tid_1,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry_polling,   /* thread entry point */
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
    trigger_semaphore,       /* thread entry point */
    &sem_1,                  /* semaphore          */
    1000,                    /* delay              */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

K_THREAD_DEFINE( 
    tid_3,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    trigger_semaphore,       /* thread entry point */
    &sem_2,                  /* semaphore          */
    1700,                    /* delay              */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

// thread 4 trigger signal
K_THREAD_DEFINE( 
    tid_4,                   /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    trigger_signal,          /* thread entry point */
    NULL,                    /* argument 1         */
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    2,                       /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

int main() {
    
    k_poll_signal_init(&poll_signal);

    return 0;
}
