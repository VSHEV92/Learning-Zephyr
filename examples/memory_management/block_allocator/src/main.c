#include <zephyr/kernel.h>
#include <zephyr/sys/mem_blocks.h>

#define BLOCKS_SIZE   256
#define BLOCKS_NUMBER 8
#define BLOCKS_ALIGN  4

// define block allocator
SYS_MEM_BLOCKS_DEFINE(
    allocator, 
    BLOCKS_SIZE, 
    BLOCKS_NUMBER, 
    BLOCKS_ALIGN
);

int main(void) {
    uintptr_t blocks[BLOCKS_NUMBER];
    uintptr_t blocks_contiguous[BLOCKS_NUMBER];

    // allocate all blocks
    printk("Allocate all blocks\n");
    sys_mem_blocks_alloc(&allocator, BLOCKS_NUMBER, (void**)blocks);

    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        printk("Block %d address: %lx\n", i, blocks[i]);
    }

    printk("Free allocated blocks\n");
    sys_mem_blocks_free(&allocator, BLOCKS_NUMBER, (void**)blocks);
    


    // allocate 3 blocks starting with block 1  
    printk("Allocate 3 blocks starting with block 1 address\n");
    sys_mem_blocks_get(&allocator, (void*)blocks[1], 3);

    // check that blocks allocated
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator, (void*)blocks[i], 1);
        if (res) {
            printk("Block %d not allocated\n", i);
        } 
        else {
            printk("Block %d allocated\n", i);
        }
    }
    
    // allocate 4 blocks continous blocks
    printk("Allocate 4 contiguous blocks\n");
    sys_mem_blocks_alloc_contiguous(&allocator, 4, (void**)blocks_contiguous);

    // check that blocks allocated
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator, (void*)blocks[i], 1);
        if (res) {
            printk("Block %d not allocated\n", i);
        } 
        else {
            printk("Block %d allocated\n", i);
        }
    }
    
    // free all regions
    printk("Free 7 allocated blocks starting from block 1\n");
    sys_mem_blocks_free_contiguous(&allocator, (void*)blocks[1], 7);

    // check that blocks freed
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator, (void*)blocks[i], 1);
        if (res) {
            printk("Block %d not allocated\n", i);
        } 
        else {
            printk("Block %d allocated\n", i);
        }
    }
    
	return 0;
}
