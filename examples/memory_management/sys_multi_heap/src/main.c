#include <zephyr/kernel.h>
#include <zephyr/sys/multi_heap.h>

#define HEAPS_SIZE 256

void* heap_choice(struct sys_multi_heap* mheap, void* cfg, size_t align, size_t size) {
    
    // find needed heap
    for(int i = 0; i < mheap->nheaps; i++) {
    
        // compare alloc cfg with heap user data
        if ( (int)cfg == (int)(mheap->heaps[i].user_data) ) {
            return sys_heap_aligned_alloc(mheap->heaps[i].heap, align, size);
        }     
    }
    return NULL;
}

int main(void) {

    // heaps structure
    struct sys_heap heap_1, heap_2;
    struct sys_multi_heap multi_heap;

    const struct sys_multi_heap_rec* multi_heap_rec;

    // heaps buffers 
    uint8_t heap_array_1[HEAPS_SIZE];
    uint8_t heap_array_2[HEAPS_SIZE];

    // initialize sys_heap
    sys_heap_init(&heap_1, heap_array_1, HEAPS_SIZE);
    sys_heap_init(&heap_2, heap_array_2, HEAPS_SIZE);

    // initialize multiheap
    sys_multi_heap_init(&multi_heap, heap_choice);
    sys_multi_heap_add_heap(&multi_heap, &heap_1, (void*)1 );
    sys_multi_heap_add_heap(&multi_heap, &heap_2, (void*)2 );

    // print heap buffer's addresses
    printk("Heap 1 buffer address: %p\n", heap_array_1);
    printk("Heap 2 buffer address: %p\n", heap_array_2);
    
    // allocate 4 bytes from first heap 
    uint8_t* mem_1 = sys_multi_heap_alloc(&multi_heap, (void*)1, 4);
    multi_heap_rec = sys_multi_heap_get_heap(&multi_heap, mem_1);
    printk( "Mem 1 address: %p. Heap: %d\n", mem_1, (int)(multi_heap_rec->user_data) );
    
    // allocate 4 bytes with alignment from second heap 
    uint8_t* mem_2 = sys_multi_heap_aligned_alloc(&multi_heap, (void*)2, 16, 4);
    multi_heap_rec = sys_multi_heap_get_heap(&multi_heap, mem_2);
    printk( "Mem 2 address: %p. Heap: %d\n", mem_2, (int)(multi_heap_rec->user_data) );

    // reallocate first buller and extend it to 32 bytes and alignment of 64 bytes from first heap 
    uint8_t* mem_3 = sys_multi_heap_aligned_realloc(&multi_heap, (void*)1, mem_1, 64, 32);
    multi_heap_rec = sys_multi_heap_get_heap(&multi_heap, mem_3);
    printk( "Mem 3 address: %p. Heap: %d\n", mem_3, (int)(multi_heap_rec->user_data) );
    
    // free memory
    sys_multi_heap_free(&multi_heap, mem_2);
    sys_multi_heap_free(&multi_heap, mem_3);
    
	return 0;
}
