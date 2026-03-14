#include <zephyr/kernel.h>
#include <zephyr/multi_heap/shared_multi_heap.h>

#define REGIONS_SIZE 256

int main(void) {
    
    // define regions buffers
    uint8_t region_1_buff[REGIONS_SIZE];
    uint8_t region_2_buff[REGIONS_SIZE];

    // Create regions
    struct shared_multi_heap_region region_1 = {
         .addr = (uintptr_t)region_1_buff,
         .size = REGIONS_SIZE,
         .attr = SMH_REG_ATTR_CACHEABLE,
    };

    struct shared_multi_heap_region region_2 = {
         .addr = (uintptr_t)region_2_buff,
         .size = REGIONS_SIZE,
         .attr = SMH_REG_ATTR_NON_CACHEABLE,
    };


    // Initialize the shared multi-heap pool
    shared_multi_heap_pool_init();


    // Add the regions to the pool with different user data
    shared_multi_heap_add(&region_1, (void*)1);
    shared_multi_heap_add(&region_2, (void*)2);


    printk("Region 1 buffer address: %p\n", region_1_buff);
    printk("Region 2 buffer address: %p\n", region_2_buff);


    // allocate 8 bytes from cachable region
    uint8_t* mem_1 = shared_multi_heap_alloc(SMH_REG_ATTR_CACHEABLE, 8);
    printk("Mem 1 address: %p\n", mem_1);

    // allocate buffer 16 bytes with 32 byte alingment from non-cachable region
    uint8_t* mem_2 = shared_multi_heap_aligned_alloc(SMH_REG_ATTR_NON_CACHEABLE, 32, 16);
    printk("Mem 2 address: %p\n", mem_2);

    // reallocate first buffer to extend it's size to 16 bytes
    uint8_t* mem_3 = shared_multi_heap_realloc(SMH_REG_ATTR_CACHEABLE, mem_1, 16);
    printk("Mem 3 address: %p\n", mem_3);

    // free memory
    shared_multi_heap_free(mem_2);
    shared_multi_heap_free(mem_3);
    
	return 0;
}
