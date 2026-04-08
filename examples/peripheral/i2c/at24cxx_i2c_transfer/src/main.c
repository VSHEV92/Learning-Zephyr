#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

#define I2C_NODE DT_NODELABEL(i2c0)

static const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

int main(void) {

    char write_offset = 0x0;
    char read_offset = 0x0;

    char read_buffer[32];

    char write_string_1[] = "Hello";
    char write_string_2[] = "Hi!!";

    struct i2c_msg i2c_msg[2];

    // transfer first string
    i2c_msg[0].buf = &write_offset;
    i2c_msg[0].len = 1;
    i2c_msg[0].flags = I2C_MSG_WRITE;

    i2c_msg[1].buf = write_string_1;
    i2c_msg[1].len = sizeof(write_string_1);
    i2c_msg[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

    i2c_transfer(i2c_dev, i2c_msg, 2, 0x50); 
    k_msleep(100);

    // read first string
    i2c_msg[0].buf = &read_offset;
    i2c_msg[0].len = 1;
    i2c_msg[0].flags = I2C_MSG_WRITE;

    i2c_msg[1].buf = read_buffer;
    i2c_msg[1].len = sizeof(write_string_1);
    i2c_msg[1].flags = I2C_MSG_READ | I2C_MSG_RESTART | I2C_MSG_STOP;

    i2c_transfer(i2c_dev, i2c_msg, 2, 0x50); 
    printk("Read from EEPROM: %s\n", read_buffer);

    // transfer second string
    i2c_msg[0].buf = &write_offset;
    i2c_msg[0].len = 1;
    i2c_msg[0].flags = I2C_MSG_WRITE;

    i2c_msg[1].buf = write_string_2;
    i2c_msg[1].len = sizeof(write_string_2);
    i2c_msg[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

    i2c_transfer(i2c_dev, i2c_msg, 2, 0x50); 
    k_msleep(100);

    // read second string
    i2c_msg[0].buf = &read_offset;
    i2c_msg[0].len = 1;
    i2c_msg[0].flags = I2C_MSG_WRITE;

    i2c_msg[1].buf = read_buffer;
    i2c_msg[1].len = sizeof(write_string_2);
    i2c_msg[1].flags = I2C_MSG_READ | I2C_MSG_RESTART | I2C_MSG_STOP;

    i2c_transfer(i2c_dev, i2c_msg, 2, 0x50); 
    printk("Read from EEPROM: %s\n", read_buffer);
   
    return 0;
}
