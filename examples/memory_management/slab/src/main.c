#include <zephyr/kernel.h>

#define SLAB_BLOCKS_SIZE    32
#define SLAB_BLOCKS_NUMBER  4

int main(void) {

    uint8_t* slab_blocks[5];

    // k_mem_slab structure
    struct k_mem_slab slab;

    // slab buffer aligned by woed size
    uint8_t __aligned(4) slab_buff[SLAB_BLOCKS_NUMBER * SLAB_BLOCKS_SIZE];

    printk("Slab buffer address: %p\n", slab_buff);

    // initialize slab
    k_mem_slab_init(&slab, slab_buff, SLAB_BLOCKS_SIZE, SLAB_BLOCKS_NUMBER);

    // try to allocate five block
    for(int i = 0; i < 5; i++) {
        int rc = k_mem_slab_alloc(&slab, (void**)&slab_blocks[i], K_NO_WAIT);
        
        if (rc == 0) {
            printk("Slab block %d address: %p\n", i, slab_blocks[i]);
        } 
        else {
            printk("Not enought block in slab\n");
        }

        printk("Used blocks in slab: %d\n", k_mem_slab_num_used_get(&slab));
    }

    // free one block
    printk("Free one block\n");
    k_mem_slab_free(&slab, slab_blocks[0]);
    printk("Used blocks in slab: %d\n", k_mem_slab_num_used_get(&slab));

    // allocate last block
    printk("Allocate another one block\n");
    k_mem_slab_alloc(&slab, (void**)&slab_blocks[4], K_NO_WAIT);
    printk("Used blocks in slab: %d\n", k_mem_slab_num_used_get(&slab));

    // free memory
    printk("Free all blocks\n");

    for(int i = 1; i < 5; i++) {
        k_mem_slab_free(&slab, slab_blocks[i]);
    }
    printk("Used blocks in slab: %d\n", k_mem_slab_num_used_get(&slab));
    
	return 0;
}
