#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>

#define MAX7219_NODE DT_NODELABEL(max7219)

static const struct device *max7219_dev = DEVICE_DT_GET(MAX7219_NODE);

int main(void) {

    uint8_t data[] = {
        0b00000000,
        0b01101100,
        0b11111110,
        0b11111110,
        0b01111100,
        0b00111000,
        0b00010000,
        0b00000000
    };

    struct display_buffer_descriptor desc = {
        .buf_size = 64,   // maxtrix is 8x8 
        .width = 8,       // width is 8
        .height = 8,      // height is 8
        .pitch = 8,       // pixels per buffer string, equal width
    };

    display_write (
        max7219_dev, // device pointer
        0,           // x coordinate 
        0,           // y coordinate
        &desc,       // buffer descriptor
        data         // buffer
    );

    uint8_t brightness = 0;
    while(true) {
        display_set_brightness(max7219_dev, brightness);
        brightness++;
        k_msleep(4);
    }

    return 0;
}
