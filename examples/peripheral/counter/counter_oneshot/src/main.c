#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h>

#define TIMER0_DEVICE DT_NODELABEL(timer0)
#define COUNTER0_DEVICE DT_CHILD(TIMER0_DEVICE, counter)

static const struct device *timer0_dev = DEVICE_DT_GET(COUNTER0_DEVICE);

void counter_alarm_cb (const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data) {
    printf("Timer alarm on channel %d triggered after %u ticks\n", chan_id, ticks); 
}

int main(void) {
    // get number of ticks to trigger alarm after 1000 usec
    uint32_t ticks = counter_us_to_ticks(timer0_dev, 1000);

    // single shot alarm config
    struct counter_alarm_cfg alarm_cfg = {
        .callback = counter_alarm_cb,
        .ticks = ticks,
        .user_data = NULL,
        .flags = 0
    };


    // start timer
    counter_start(timer0_dev);

    while(true) {
        // reset counter
        counter_reset(timer0_dev);
        
        // set alarm for channel 0
        counter_set_channel_alarm(timer0_dev, 0, &alarm_cfg);

        k_msleep(1000);
    }

	return 0;
}
