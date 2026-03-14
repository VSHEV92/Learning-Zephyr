#include <zephyr/kernel.h>
#include <zephyr/sys/mem_blocks.h>

#define BLOCKS_NUMBER 4
#define BLOCKS_ALIGN  4

// define block allocators
SYS_MEM_BLOCKS_DEFINE(
    allocator_0, 
    32, 
    BLOCKS_NUMBER, 
    BLOCKS_ALIGN
);

SYS_MEM_BLOCKS_DEFINE(
    allocator_1, 
    64, 
    BLOCKS_NUMBER, 
    BLOCKS_ALIGN
);

sys_mem_blocks_t* allocator_choice(struct sys_multi_mem_blocks *group, void *cfg) {

    if ( (int)cfg == 0 ) {
        return &allocator_0;
    }

    if ( (int)cfg == 1 ) {
        return &allocator_1;
    }
    
    return 0;
}

int main(void) {
    size_t blocks_size;
    uintptr_t blocks[BLOCKS_NUMBER];

    // initialize allocator group structure
    sys_multi_mem_blocks_t allocator_group;
    sys_multi_mem_blocks_init(&allocator_group, allocator_choice);

    // add allocators to group
    sys_multi_mem_blocks_add_allocator(&allocator_group, &allocator_0);
    sys_multi_mem_blocks_add_allocator(&allocator_group, &allocator_1); 

	
    // -----------------------------------------------------------------------
    // allocate all blocks to get there addresses
    uintptr_t blocks_0[BLOCKS_NUMBER];
    sys_mem_blocks_alloc(&allocator_0, BLOCKS_NUMBER, (void**)blocks_0);
    sys_mem_blocks_free(&allocator_0, BLOCKS_NUMBER, (void**)blocks_0);

    uintptr_t blocks_1[BLOCKS_NUMBER];
    sys_mem_blocks_alloc(&allocator_1, BLOCKS_NUMBER, (void**)blocks_1);
    sys_mem_blocks_free(&allocator_1, BLOCKS_NUMBER, (void**)blocks_1);


    // -----------------------------------------------------------------------

    printk("Allocate 2 blocks from allocator 0\n");
    sys_multi_mem_blocks_alloc(
        &allocator_group,  // group  
        (void*)0,          // choice config
        2,                 // blocks number
        (void**)blocks,    // addresses
        &blocks_size       // actual block size
    );
    printk("Allocator 0 blocks size: %u\n", blocks_size);

    // check that blocks allocated
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_0, (void*)blocks_0[i], 1);
        if (res) {
            printk("Allocator 0 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 0 Block %d allocated\n", i);
        }
    }

    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_1, (void*)blocks_1[i], 1);
        if (res) {
            printk("Allocator 1 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 1 Block %d allocated\n", i);
        }
    }
    

    printk("Free allocator 0 blocks\n");
    sys_multi_mem_blocks_free(
        &allocator_group,  // group  
        2,                 // blocks number
        (void**)blocks    // addresses
    );

    printk("Allocator 0 blocks size: %u\n", blocks_size);

    // check that blocks allocated
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_0, (void*)blocks_0[i], 1);
        if (res) {
            printk("Allocator 0 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 0 Block %d allocated\n", i);
        }
    }

    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_1, (void*)blocks_1[i], 1);
        if (res) {
            printk("Allocator 1 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 1 Block %d allocated\n", i);
        }
    }


    // -----------------------------------------------------------------------

    printk("Allocate 3 blocks from allocator 1\n");
    sys_multi_mem_blocks_alloc(
        &allocator_group,  // group  
        (void*)1,          // choice config
        3,                 // blocks number
        (void**)blocks,    // addresses
        &blocks_size       // actual block size
    );
    printk("Allocator 1 blocks size: %u\n", blocks_size);

    // check that blocks allocated
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_0, (void*)blocks_0[i], 1);
        if (res) {
            printk("Allocator 0 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 0 Block %d allocated\n", i);
        }
    }

    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_1, (void*)blocks_1[i], 1);
        if (res) {
            printk("Allocator 1 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 1 Block %d allocated\n", i);
        }
    }
    

    printk("Free allocator 1 blocks\n");
    sys_multi_mem_blocks_free(
        &allocator_group,  // group  
        3,                 // blocks number
        (void**)blocks    // addresses
    );

    printk("Allocator 0 blocks size: %u\n", blocks_size);

    // check that blocks allocated
    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_0, (void*)blocks_0[i], 1);
        if (res) {
            printk("Allocator 0 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 0 Block %d allocated\n", i);
        }
    }

    for(int i = 0; i < BLOCKS_NUMBER; i++) {
        int res = sys_mem_blocks_is_region_free(&allocator_1, (void*)blocks_1[i], 1);
        if (res) {
            printk("Allocator 1 Block %d not allocated\n", i);
        } 
        else {
            printk("Allocator 1 Block %d allocated\n", i);
        }
    }



    return 0;
}
