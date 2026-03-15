#include <zephyr/kernel.h>
#include <zephyr/sys/dlist.h>

#define LIST_NODES 4

struct dlist_data_t{
    sys_dnode_t node;
    int data;
};


int main() {
    size_t list_len;
    
    sys_dnode_t *node;

    struct dlist_data_t *list_data_ptr;
    struct dlist_data_t list_data[LIST_NODES];

    // create double linked list
    sys_dlist_t dlist;
    sys_dlist_init(&dlist);	

    // --------------------------------------------------------------------
    // append data to list
    for(int i = 0; i < LIST_NODES; i++) {
        list_data[i].data = i;
        sys_dlist_append( &dlist, &(list_data[i].node) );
    }

    // iterate through list and print nodes
    SYS_DLIST_FOR_EACH_CONTAINER(&dlist, list_data_ptr, node) {
        printf("List data: %d\n", list_data_ptr->data);
    }

    // iterate through list, get data and remove it from list
    list_len = sys_dlist_len(&dlist);

    for(int i = 0; i < list_len; i++) {
        sys_dnode_t *dlist_node = sys_dlist_get(&dlist);
        list_data_ptr = CONTAINER_OF(dlist_node, struct dlist_data_t, node);
        printf("Get list data: %d\n", list_data_ptr->data);
    }

    // --------------------------------------------------------------------
    // prepend data to list then get it
    printk("Prepend data to list\n");
    for(int i = 0; i < LIST_NODES; i++) {
        list_data[i].data = i;
        sys_dlist_prepend( &dlist, &(list_data[i].node) );
    }

    // iterate through list, get data and remove it from list
    list_len = sys_dlist_len(&dlist);

    for(int i = 0; i < list_len; i++) {
        sys_dnode_t *dlist_node = sys_dlist_get(&dlist);
        list_data_ptr = CONTAINER_OF(dlist_node, struct dlist_data_t, node);
        printf("Get list data: %d\n", list_data_ptr->data);
    }

    // --------------------------------------------------------------------
    // create another list and merge it with first
 
    // append data to list
    for(int i = 0; i < LIST_NODES-2; i++) {
        list_data[i].data = i;
        sys_dlist_append( &dlist, &(list_data[i].node) );
    }
 
    // create other linked list
    sys_dlist_t dlist_other;
    sys_dlist_init(&dlist_other);	
 
    // add data to other list
    list_data[LIST_NODES-2].data = 101;
    sys_dlist_append( &dlist_other, &(list_data[LIST_NODES-2].node) );
    
    list_data[LIST_NODES-1].data = 102;
    sys_dlist_append( &dlist_other, &(list_data[LIST_NODES-1].node) );
 
    // merge lists
    printk("Merge lists\n");

    sys_dnode_t *dlist_ohter_head = sys_dlist_peek_head(&dlist_other);
    sys_dnode_t *dlist_ohter_tail = sys_dlist_peek_tail(&dlist_other);
    sys_dlist_range_append(&dlist, dlist_ohter_head, dlist_ohter_tail);
 
    // iterate through list and print nodes
    SYS_DLIST_FOR_EACH_CONTAINER(&dlist, list_data_ptr, node) {
        printf("List data: %d\n", list_data_ptr->data);
    }
 
 
    // --------------------------------------------------------------------
    // iterate throught list using peek methods
    node = sys_dlist_peek_head(&dlist);
    while(node != NULL) {
        list_data_ptr = CONTAINER_OF(node, struct dlist_data_t, node);
        printf("Peek forward list data: %d\n", list_data_ptr->data);
        node = sys_dlist_peek_next(&dlist, node);
    }


    node = sys_dlist_peek_tail(&dlist);
    while(node != NULL) {
        list_data_ptr = CONTAINER_OF(node, struct dlist_data_t, node);
        printf("Peek backward list data: %d\n", list_data_ptr->data);
        node = sys_dlist_peek_prev(&dlist, node);
    }

    return 0;
}
