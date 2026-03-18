#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>

#define RING_BUFF_SIZE 13

struct ring_buf rbuf;
uint8_t buff[RING_BUFF_SIZE];

int main() {
    size_t size;

    // init test data array
    uint8_t* tx_data;
    uint8_t* rx_data;

    uint8_t peek_data[RING_BUFF_SIZE];

    // init ring buffer
    ring_buf_init(&rbuf, RING_BUFF_SIZE, buff);
    
    // -----------------------------------------------------
    // put claim bytes data to buffer
    size = ring_buf_put_claim(&rbuf, &tx_data, 5);
    printk("Put claim to buffer data of size %d\n", size);

    for(int i = 0; i < size; i++) {
        tx_data[i] = i;   
    }

    // put other claim bytes data to buffer
    size = ring_buf_put_claim(&rbuf, &tx_data, 5);
    printk("Put claim to buffer data of size %d\n", size);

    for(int i = 0; i < size; i++) {
        tx_data[i] = i + 5;   
    }

    // finish put 
    ring_buf_put_finish(&rbuf, 10);

    size = ring_buf_size_get(&rbuf);
    printk("Size of data in buffer: %d\n", size);

    // -----------------------------------------------------
    // peek 10 bytes from ring buffer
    size = ring_buf_peek(&rbuf, peek_data, 10);
    printk("Peek from buffer data of size %d\n", size);
    
    for (int i = 0; i < size; i++) {
        printk("RX data[%d]: %d\n", i, peek_data[i]);
    }

    // -----------------------------------------------------
    // get claim 3 bytes from ring buffer
    size = ring_buf_get_claim(&rbuf, &rx_data, 3);
    printk("Get claim from buffer data of size %d\n", size);
    
    for (int i = 0; i < size; i++) {
        printk("RX data[%d]: %d\n", i, rx_data[i]);
    }

    // other get claim of 7 bytes from ring buffer
    size = ring_buf_get_claim(&rbuf, &rx_data, 7);
    printk("Get claim from buffer data of size %d\n", size);
    
    for (int i = 0; i < size; i++) {
        printk("RX data[%d]: %d\n", i, rx_data[i]);
    }

    // finish get
    ring_buf_get_finish(&rbuf, 10);

    size = ring_buf_space_get(&rbuf);
    printk("Buffer free size: %d\n", size);

    return 0;
}
