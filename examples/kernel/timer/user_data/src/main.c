#include <zephyr/kernel.h>

struct timer_data_t {
    int idx;
    int count;
};

void timer_func(struct k_timer *timer_id) {
    struct timer_data_t* timer_data;

    // get timer data
    timer_data = (struct timer_data_t*) k_timer_user_data_get(timer_id);

    // print message and increment counter
    printk("Timer %d fired %d times\n", timer_data->idx, timer_data->count);
    (timer_data->count)++;
}

static struct k_timer timer_1, timer_2;

static struct timer_data_t timer_1_data, timer_2_data;

int main() {
    
    // initialize timers
    k_timer_init(&timer_1, timer_func, NULL);
    k_timer_init(&timer_2, timer_func, NULL);

    // start timers
    k_timer_start( &timer_1, K_MSEC( 1700 ), K_MSEC( 1700 ) );
    k_timer_start( &timer_2, K_MSEC( 2400 ), K_MSEC( 2400 ) );
    
    // set timer data
    timer_1_data.idx = 1;
    timer_1_data.count = 0;

    timer_2_data.idx = 2;
    timer_2_data.count = 0;

    k_timer_user_data_set(&timer_1, &timer_1_data);
    k_timer_user_data_set(&timer_2, &timer_2_data);

    return 0;
}

