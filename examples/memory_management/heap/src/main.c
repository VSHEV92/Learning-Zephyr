#include <zephyr/kernel.h>

#define HEAP_SIZE 256

int main(void) {

    // k_heap structure
    struct k_heap heap;

    // heap buffer aligned by it's size 
    uint8_t __aligned(HEAP_SIZE) heap_array[HEAP_SIZE];

    printk("Heap buffer address: %p\n", heap_array);

    // initialize heap
    k_heap_init(&heap, heap_array, HEAP_SIZE);

    // allocate 8 bytes
    uint8_t* mem_1 = k_heap_alloc(&heap, 8, K_FOREVER);
    printk("Mem 1 address: %p\n", mem_1);

    // allocate 4 bytes and initialize them to zeros
    uint8_t* mem_2 = k_heap_calloc(&heap, 4, 1, K_FOREVER);
    printk("Mem 2 address: %p\n", mem_2);
    printk("Mem 2 data: %d %d %d %d\n", mem_2[0], mem_2[1], mem_2[2], mem_2[3]);

    // reallocate first buffer to extend it's size to 16 bytes
    uint8_t* mem_3 = k_heap_realloc(&heap, mem_1, 16, K_FOREVER);
    printk("Mem 3 address: %p\n", mem_3);

    // allocate buffer with alingment
    uint8_t* mem_4 = k_heap_aligned_alloc(&heap, 32, 24, K_FOREVER);
    printk("Mem 4 address: %p\n", mem_4);

    // free memory
    k_heap_free(&heap, mem_2);
    k_heap_free(&heap, mem_3);
    k_heap_free(&heap, mem_4);
    
	return 0;
}
