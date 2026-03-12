#include <zephyr/kernel.h>

int main(void) {

    // allocate 8 bytes
    uint8_t* mem_1 = k_malloc(8);
    printk("Mem 1 address: %p\n", mem_1);

    // allocate 4 bytes and initialize them to zeros
    uint8_t* mem_2 = k_calloc(4, 1);
    printk("Mem 2 address: %p\n", mem_2);
    printk("Mem 2 data: %d %d %d %d\n", mem_2[0], mem_2[1], mem_2[2], mem_2[3]);

    // reallocate first buffer to extend it's size to 16 bytes
    uint8_t* mem_3 = k_realloc(mem_1, 16);
    printk("Mem 3 address: %p\n", mem_3);

    // allocate buffer with alingment
    uint8_t* mem_4 = k_aligned_alloc(32, 24);
    printk("Mem 4 address: %p\n", mem_4);

    // free memory
    k_free(mem_2);
    k_free(mem_3);
    k_free(mem_4);
    
	return 0;
}
