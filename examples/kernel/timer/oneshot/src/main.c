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

    for (int i = 0; i < 5; i++) {

        // start timer
        k_timer_start(
            &timer,          // timer pointer
            K_MSEC( 100 ),   // initial delay
            K_FOREVER        // periodic delay, forever for one shot
        );
    
        k_msleep( 1000 );
    }
    
    return 0;
}

