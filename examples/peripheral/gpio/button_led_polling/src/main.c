#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS   1000

// get dt nodes base on alises names
#define LED_NODE DT_ALIAS(led0)
#define BUTTON_NODE DT_ALIAS(sw0)

// get gpio dt structs
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

int main(void) {
	bool button_state = true;

    // check if gpios are ready
	if (!gpio_is_ready_dt(&led)) { printk("GPIO LED is not ready!\n"); return 0; }
	if (!gpio_is_ready_dt(&button)) { printk("GPIO BUTTON is not ready!\n"); return 0; }

    // configure gpios
	gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	gpio_pin_configure_dt(&button, GPIO_INPUT | GPIO_ACTIVE_LOW);

	while (1) {
        // get value for button
		button_state = gpio_pin_get_dt(&button);

        // set led value
		gpio_pin_set_dt(&led, button_state);
	}

	return 0;
}
