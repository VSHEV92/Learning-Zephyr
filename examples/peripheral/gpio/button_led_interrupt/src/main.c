#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// get dt nodes base on alises names
#define LED_NODE DT_ALIAS(led0)
#define BUTTON_NODE DT_ALIAS(sw0)

// get gpio dt structs
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

// button gpio callback data structure
static struct gpio_callback button_callback_data;

// buttom interrupt handler
/* Parameters:
        - device structire 
        - gpio callback data structure (handler, interested pin mask)
        - pins, that trigger handler 
*/
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    gpio_pin_toggle_dt(&led);
}

int main(void) {

    // Configure gpios. Set led as output, and button as input
	gpio_pin_configure_dt(&led, GPIO_OUTPUT | led.dt_flags);
	gpio_pin_configure_dt(&button, GPIO_INPUT | button.dt_flags);

    // Configure gpio button interrupt. Trigger when button edges to active level
	gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    
    // Configure gpio callback data structure
    /* Parameters:
        - gpio callback data structure that will be initialized
        - callback handler function
        - bit mask of gpio pins, that trigger handler. Set to button pin
    */
	gpio_init_callback(&button_callback_data, button_pressed, BIT(button.pin));

    // Add callback to device stricture
    /* Parameters:
        - Device structure. Get it from gpio_dt_spec structure
        - Earlier configured gpio callback data structure
    */
	gpio_add_callback(button.port, &button_callback_data);

    // Dummy loop
	while (1) {}

	return 0;
}
