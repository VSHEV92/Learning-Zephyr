#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// gpio settings specific to espc3 supermini board 
# define GPIO_LED_PIN 8
# define GPIO_BUTTON_PIN 9

// get dt nodes base on alises names
#define GPIO_NODE DT_NODELABEL(gpio0)

// get gpio dt structs
static const struct device* gpio_dev = DEVICE_DT_GET(GPIO_NODE);

int main(void) {
	bool button_state = true;

    // Configure gpios. Use device struct, pin number and gpio flags
	gpio_pin_configure(gpio_dev, GPIO_LED_PIN, GPIO_OUTPUT | GPIO_ACTIVE_LOW);
	gpio_pin_configure(gpio_dev, GPIO_BUTTON_PIN, GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_LOW);

	while (1) {
        // get value for button
		button_state = gpio_pin_get(gpio_dev, GPIO_BUTTON_PIN);

        // set led value
		gpio_pin_set(gpio_dev, GPIO_LED_PIN, button_state);
	}

	return 0;
}
