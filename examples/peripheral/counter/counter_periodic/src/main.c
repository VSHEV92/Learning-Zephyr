#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/counter.h>

#define TIMER0_DEVICE DT_NODELABEL(timer0)
#define COUNTER0_DEVICE DT_CHILD(TIMER0_DEVICE, counter)

#define LED_NODE DT_ALIAS(led0)

static const struct device *timer0_dev = DEVICE_DT_GET(COUNTER0_DEVICE);

static const struct led_dt_spec led = LED_DT_SPEC_GET(LED_NODE);

static bool state = 0;

void counter_top_cb (const struct device *dev, void *user_data) {
    (state) ? led_off_dt(&led) : led_on_dt(&led);
    state = !state;
}

int main(void) {
    // turn led off
    led_off_dt(&led);

    // get number of ticks to trigger alarm after 1000 msec
    uint32_t ticks = counter_us_to_ticks(timer0_dev, 1000000);

    // single shot alarm config
    struct counter_top_cfg top_cfg = {
        .callback = counter_top_cb,
        .ticks = ticks,
        .user_data = NULL,
        .flags = 0
    };

    // reset counter
    counter_reset(timer0_dev);
    
    // set top value
    counter_set_top_value(timer0_dev, &top_cfg);

    // start timer
    counter_start(timer0_dev);


	return 0;
}
