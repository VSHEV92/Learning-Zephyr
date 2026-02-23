#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/input/input.h>


// get dt nodes base on alises names
#define LED_NODE DT_ALIAS(led0)
#define GPIO_KEYS_NODE DT_PATH(gpio_keys)

// get led dt structs
static const struct led_dt_spec led = LED_DT_SPEC_GET(LED_NODE);

// button event callback
// Get input_event structure 
//  - device handler, 
//  - type code: key event, abs value and so on
//  - sync flag
//  - key value
static void button_cb(struct input_event *evt, void *user_data) {
    if (evt->value) {
	    led_on_dt(&led);
    }
    else {
	    led_off_dt(&led);
    }
}

// set button callback
/* Parameters:
    - device, that can trigger callback
    - callback function
    - user data pointer
*/
INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(GPIO_KEYS_NODE), button_cb, NULL);

int main(void) {

	k_sleep(K_FOREVER);

	return 0;
}
