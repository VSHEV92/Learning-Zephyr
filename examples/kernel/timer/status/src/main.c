#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024

static struct k_timer timer;

struct k_thread thread;
k_thread_stack_t* thread_stack;

void threads_entry(void * arg_1, void * arg_2, void * arg_3) {
    while(1) {
        printk("Thread wait for a timer\n");
        k_timer_status_sync(&timer);	
        printk("Thread unblocked\n");
    }
}

void timer_func(struct k_timer *timer_id) {
    printk("Timer fired\n");
}

int main() {
    
    // initialize timer
    k_timer_init(
        &timer,      // timer pointer
        timer_func,  // expiry function
        NULL         // stop function
    );

    // start timer
    k_timer_start(
        &timer,          // timer pointer
        K_MSEC( 1000 ),  // initial delay
        K_MSEC( 2000 )   // periodic delay
    );
    
    // start thread
    thread_stack = k_thread_stack_alloc(THREAD_STACK_SIZE, 0);

    k_thread_create(
        &thread,                                // thread control block 
        thread_stack,                           // thread stack 
        THREAD_STACK_SIZE,                      // stack size
        threads_entry,                          // thread entry point
        NULL,                                   // argument 1
        NULL,                                   // argument 2
        NULL,                                   // argument 3
        1,                                      // thread priority
        0,                                      // thread options
        K_NO_WAIT                               // delay before thread start
    );
    
    k_sleep(K_FOREVER);

    return 0;
}

