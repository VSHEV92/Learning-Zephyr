#include <zephyr/kernel.h>

#define HEAP_SIZE 256

int main(void) {

    // sys_heap structure
    struct sys_heap	heap;

    // heap buffer aligned by it's size 
    uint8_t __aligned(HEAP_SIZE) heap_array[HEAP_SIZE];

    printk("Heap buffer address: %p\n", heap_array);

    // initialize heap
    sys_heap_init(&heap, heap_array, HEAP_SIZE);

    // allocate 4 bytes
    uint8_t* mem_1 = sys_heap_alloc(&heap, 4);
    printk("Mem 1 address: %p\n", mem_1);

    // allocate 4 bytes with 16 byte alignment
    uint8_t* mem_2 = sys_heap_aligned_alloc(&heap, 16, 4);
    printk("Mem 2 address: %p\n", mem_2);

    // reallocate first buffer to extend it's size to 16 bytes
    uint8_t* mem_3 = sys_heap_realloc(&heap, mem_1, 16);
    printk("Mem 3 address: %p\n", mem_3);

    // reallocate second buffer
    uint8_t* mem_4 = sys_heap_aligned_realloc(&heap, mem_2, 32, 24);
    printk("Mem 4 address: %p\n", mem_4);

    // print memory sizes
    printk("Mem 3 size: %u\n", sys_heap_usable_size(&heap, mem_3));
    printk("Mem 4 size: %u\n", sys_heap_usable_size(&heap, mem_4));

    // free memory
    sys_heap_free(&heap, mem_3);
    sys_heap_free(&heap, mem_4);
    
	return 0;
}
