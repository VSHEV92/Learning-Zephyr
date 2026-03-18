#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>

#define RING_BUFF_SIZE 13

struct ring_buf rbuf;
uint32_t buff[RING_BUFF_SIZE];

int main() {
    uint8_t size;
    int rc = 0;
    
    uint8_t value;
    uint16_t type;


    // init test data array
    uint32_t tx_data[2*RING_BUFF_SIZE];
    uint32_t rx_data[2*RING_BUFF_SIZE];

    for(int i = 0; i < 2*RING_BUFF_SIZE; i++) {
        tx_data[i] = i;
    }

    // init ring buffer
    ring_buf_item_init(&rbuf, RING_BUFF_SIZE, buff);
    
    // check buffer capacity
    size = ring_buf_capacity_get(&rbuf);
    printk("Available space in ring buffer (bytes): %d\n", size);

    // put 10 bytes data to buffer
    rc = ring_buf_item_put(&rbuf, 0, 0, tx_data, 10);
    if (!rc) {
        printk("Put to buffer data of size %d\n", 10);
    }
    else {
        printk("Put to buffer data failed\n");
    }

    // try to put another 10 bytes data to buffer
    rc = ring_buf_item_put(&rbuf, 0, 0, tx_data + 10, 10);
    if (!rc) {
        printk("Put to buffer data of size %d\n", 10);
    }
    else {
        printk("Put to buffer data failed\n");
    }

    // get bytes from ring buffer to 7 element array
    size = 7;
    rc = ring_buf_item_get(&rbuf, &type, &value, rx_data, &size);
    if (rc == -EMSGSIZE) {
        printk("Buffer is too small, need %d uint32_t\n", size);
    } else if (rc == -EAGAIN) {
        printk("Ring buffer is empty\n");
    } else {
        printk("Get from buffer data of size %d\n", size);
    }

    // get bytes from ring buffer to 15 element array
    size = 15;
    rc = ring_buf_item_get(&rbuf, &type, &value, rx_data, &size);
    if (rc == -EMSGSIZE) {
        printk("Buffer is too small, need %d uint32_t\n", size);
    } else if (rc == -EAGAIN) {
        printk("Ring buffer is empty\n");
    } else {
        printk("Get from buffer data of size %d\n", size);
    }

    for (int i = 0; i < size; i++) {
        printk("RX data[%d]: %d\n", i, rx_data[i]);
    }

    size = ring_buf_item_space_get(&rbuf);
    printk("Buffer free size: %d\n", size);

    return 0;
}
