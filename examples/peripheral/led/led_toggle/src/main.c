#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>

// set toggle time to 500 msec
#define TOGGLE_TIME_MS 500

// get dt nodes base on alises names
#define LED_NODE DT_ALIAS(led0)

// get led dt structs
static const struct led_dt_spec led = LED_DT_SPEC_GET(LED_NODE);

int main(void) {

    // Check if led are ready. 
	if (!led_is_ready_dt(&led)) { printk("LED is not ready!\n"); return 0; }

    // toggle led every toggle time
	while (1) {
        led_on_dt(&led);
		k_msleep(TOGGLE_TIME_MS);

        led_off_dt(&led);
		k_msleep(TOGGLE_TIME_MS);
	}

	return 0;
}
