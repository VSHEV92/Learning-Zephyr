#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>

#define RING_BUFF_SIZE 13

struct ring_buf rbuf;
uint8_t buff[RING_BUFF_SIZE];

int main() {
    size_t size;

    // init test data array
    uint8_t tx_data[2*RING_BUFF_SIZE];
    uint8_t rx_data[2*RING_BUFF_SIZE];

    for(int i = 0; i < 2*RING_BUFF_SIZE; i++) {
        tx_data[i] = i;
    }

    // init ring buffer
    ring_buf_init(&rbuf, RING_BUFF_SIZE, buff);
    
    // check buffer capacity
    size = ring_buf_capacity_get(&rbuf);
    printk("Available space in ring buffer: %d\n", size);

    // put 10 bytes data to buffer
    size = ring_buf_put(&rbuf, tx_data, 10);
    printk("Put to buffer data of size %d\n", size);

    size = ring_buf_size_get(&rbuf);
    printk("Size of data in buffer: %d\n", size);

    // try to put another 10 bytes data to buffer
    size = ring_buf_put(&rbuf, tx_data + 10, 10);
    printk("Put to buffer data of size %d\n", size);

    size = ring_buf_size_get(&rbuf);
    printk("Size of data in buffer: %d\n", size);

    // get 10 bytes from ring buffer
    size = ring_buf_get(&rbuf, rx_data, 10);
    printk("Get from buffer data of size %d\n", size);
    
    for (int i = 0; i < size; i++) {
        printk("RX data[%d]: %d\n", i, rx_data[i]);
    }

    // put another 7 bytes data to buffer
    size = ring_buf_put(&rbuf, tx_data + 13, 7);
    printk("Put to buffer data of size %d\n", size);
     
    // get 5 bytes from ring buffer
    size = ring_buf_get(&rbuf, rx_data, 5);
    printk("Get another from buffer data of size %d\n", size);
    
    for (int i = 0; i < size; i++) {
        printk("RX data[%d]: %d\n", i, rx_data[i]);
    }

    // reset ring buffer
    size = ring_buf_size_get(&rbuf);
    printk("Size of data in buffer: %d\n", size);

    size = ring_buf_space_get(&rbuf);
    printk("Current buffer free size: %d\n", size);

    ring_buf_reset(&rbuf);
    size = ring_buf_space_get(&rbuf);
    printk("Buffer free size after reset: %d\n", size);

    return 0;
}
