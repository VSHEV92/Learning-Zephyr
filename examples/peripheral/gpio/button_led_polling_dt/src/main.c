#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// get dt nodes base on alises names
#define LED_NODE DT_ALIAS(led0)
#define BUTTON_NODE DT_ALIAS(sw0)

/*
    gpio_dt_spec - Container for GPIO pin information specified in devicetree.
        - dt_flags: pin's configuration flags as specified in devicetree
        - pin:      gpio pin number
        - port:     GPIO device struct controlling the pin.

    GPIO_DT_SPEC_GET - return gpio_dt_spec. Get as paramenter
        - DT NODE 
        - phandle-array of gpios. Get 0 index gpio
*/

// get gpio dt structs
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

int main(void) {
	bool button_state = true;

    // Check if gpios are ready. Call device_is_ready(), that just check that gpio_dt_spec 
    // pointer is not null and device is initialized
	if (!gpio_is_ready_dt(&led)) { printk("GPIO LED is not ready!\n"); return 0; }
	if (!gpio_is_ready_dt(&button)) { printk("GPIO BUTTON is not ready!\n"); return 0; }

    // Configure gpios. Set led as output, and button as input
	gpio_pin_configure_dt(&led, GPIO_OUTPUT | led.dt_flags);
	gpio_pin_configure_dt(&button, GPIO_INPUT | button.dt_flags);

	while (1) {
        // Get button state
		button_state = gpio_pin_get_dt(&button);

        // Set led value
		gpio_pin_set_dt(&led, button_state);
	}

	return 0;
}
