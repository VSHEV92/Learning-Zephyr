#include <zephyr/kernel.h>
#include <zephyr/sys/min_heap.h>

#define HEAP_CAPACITANCE  10 
#define HEAP_ELEMENT_SIZE sizeof(int)


int min_heap_cmp_funct(const void *a, const void *b) {
    return *( (int*)a ) - *( (int*)b );
}


int main() {
    bool empty;
    int heap_data[] = {1, 5, -7, 13, -1, 10, -11};
    int heap_buffer[HEAP_CAPACITANCE];
    void* heap_element = NULL;
    int data;

    // create min heap
    struct min_heap heap;
    min_heap_init(&heap, heap_buffer, HEAP_CAPACITANCE, HEAP_ELEMENT_SIZE, min_heap_cmp_funct);

    // check if heap is empty
    empty = min_heap_is_empty(&heap);
    printk("Heap is %s empty\n", empty ? "" : "not");

    // add elements to heap
    printk("Add elements to heap\n");
    for(int i = 0; i < sizeof(heap_data)/sizeof(heap_data[0]); i++) {
        min_heap_push(&heap, &heap_data[i]);
    }

    // check again if heap is empty
    empty = min_heap_is_empty(&heap);
    printk("Heap is %s empty\n", empty ? "" : "not");

    // iterate throught heap and print elemtnts
    printk("Iterate throught heap\n");
    MIN_HEAP_FOREACH(&heap, heap_element) {
        printk("Heap element: %d\n", *( (int*)heap_element ));
    }
    
    // remove some elements
    printk("Remove heap elements\n");

    min_heap_remove(&heap, 0, &data);
    printk("Removed element: %d\n", data);

    min_heap_remove(&heap, 1, &data);
    printk("Removed element: %d\n", data);

    // get elements from heap
    printk("Get heap elements\n");
    for(int i = 0; i < sizeof(heap_data)/sizeof(heap_data[0]) - 2; i++) {
        heap_element = min_heap_get_element(&heap, i);
        printk("Poped element: %d\n", *( (int*)heap_element ));
    }
    
    // pop elements from heap
    printk("Pop elements from heap\n");
    for(int i = 0; i < sizeof(heap_data)/sizeof(heap_data[0]) - 2; i++) {
        min_heap_pop(&heap, &data);
        printk("Poped element: %d\n", data);
    }

    // check again if heap is empty
    empty = min_heap_is_empty(&heap);
    printk("Heap is %s empty\n", empty ? "" : "not");

    return 0;

}
