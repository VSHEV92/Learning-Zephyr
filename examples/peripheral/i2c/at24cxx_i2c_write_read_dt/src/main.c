#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

#define EEPROM_NODE DT_NODELABEL(eeprom)

static const struct i2c_dt_spec eeprom_spec = I2C_DT_SPEC_GET(EEPROM_NODE);

int main(void) {

    char write_offset = 0x0;
    char read_offset = 0x0;

    char write_buffer[32];
    char read_buffer[32];

    char write_string_1[] = "Hello";
    char write_string_2[] = "Hi!!";

    // write first string
    write_buffer[0] = write_offset;
    strcpy(write_buffer + 1, write_string_1);

    i2c_write_dt(&eeprom_spec, write_buffer, 1 + sizeof(write_string_1)); 
    k_msleep(100);

    // read first string
    write_buffer[0] = read_offset;

    i2c_write_read_dt(&eeprom_spec, write_buffer, 1, read_buffer, sizeof(write_string_1)); 
    printk("Read from EEPROM: %s\n", read_buffer);
    

    // write second string
    write_buffer[0] = write_offset;
    strcpy(write_buffer + 1, write_string_2);

    i2c_write_dt(&eeprom_spec, write_buffer, 1 + sizeof(write_string_2)); 
    k_msleep(100);

    // read second string
    write_buffer[0] = read_offset;

    i2c_write_read_dt(&eeprom_spec, write_buffer, 1, read_buffer, sizeof(write_string_2)); 
    printk("Read from EEPROM: %s\n", read_buffer);
   
    return 0;
}
