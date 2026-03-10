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

    printk("Start timer\n");
    
    // start timer
    k_timer_start(
        &timer,          // timer pointer
        K_MSEC( 1000 ),  // initial delay
        K_MSEC( 1000 )   // periodic delay
    );
    
    // wait for some delay and stop timer
    k_msleep(6000);

    printk("Stop timer\n");
    k_timer_stop(&timer);	
    
    return 0;
}

