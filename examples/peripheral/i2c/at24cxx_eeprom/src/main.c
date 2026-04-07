#include <zephyr/kernel.h>
#include <zephyr/drivers/eeprom.h>

#define EEPROM_NODE DT_NODELABEL(eeprom)

static const struct device *eeprom_dev = DEVICE_DT_GET(EEPROM_NODE);

int main(void) {

    char read_buffer[32];

    char write_string_1[] = "Hello, Zephyr";
    char write_string_2[] = "I'm EEPROM";

    // write and read second string
    eeprom_write(eeprom_dev, 0, write_string_1, sizeof(write_string_1));
    k_msleep(100);

    eeprom_read(eeprom_dev, 0, read_buffer, sizeof(write_string_1));
    printk("Read from EEPROM: %s\n", read_buffer);
    
    // write and read second string
    eeprom_write(eeprom_dev, 0, write_string_2, sizeof(write_string_2));
    k_msleep(100);

    eeprom_read(eeprom_dev, 0, read_buffer, sizeof(write_string_2));
    printk("Read from EEPROM: %s\n", read_buffer);


    return 0;
}
