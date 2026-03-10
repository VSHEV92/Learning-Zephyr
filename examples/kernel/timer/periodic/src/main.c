#include <zephyr/kernel.h>


void timer_func(struct k_timer *timer_id) {
    printk("Timer fired\n");
}

static struct k_timer timer;

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
        K_MSEC( 1000 )   // periodic delay
    );
    
    
    return 0;
}

