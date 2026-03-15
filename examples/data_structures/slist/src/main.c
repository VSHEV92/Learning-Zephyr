#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>

#define LIST_NODES 4

struct slist_data_t{
    sys_snode_t node;
    int data;
};


int main() {
    size_t list_len;

    struct slist_data_t *list_data_ptr;
    struct slist_data_t list_data[LIST_NODES];

    // create single linked list
    sys_slist_t slist;
    sys_slist_init(&slist);	

    // --------------------------------------------------------------------
    // append data to list
    for(int i = 0; i < LIST_NODES; i++) {
        list_data[i].data = i;
        sys_slist_append( &slist, &(list_data[i].node) );
    }

    // iterate through list and print nodes
    SYS_SLIST_FOR_EACH_CONTAINER(&slist, list_data_ptr, node) {
        printf("List data: %d\n", list_data_ptr->data);
    }

    // iterate through list, get data and remove it from list
    list_len = sys_slist_len(&slist);

    for(int i = 0; i < list_len; i++) {
        sys_snode_t *slist_node = sys_slist_get(&slist);
        list_data_ptr = CONTAINER_OF(slist_node, struct slist_data_t, node);
        printf("Get list data: %d\n", list_data_ptr->data);
    }

    // --------------------------------------------------------------------
    // prepend data to list then get it
    printk("Prepend data to list\n");
    for(int i = 0; i < LIST_NODES; i++) {
        list_data[i].data = i;
        sys_slist_prepend( &slist, &(list_data[i].node) );
    }

    // iterate through list, get data and remove it from list
    list_len = sys_slist_len(&slist);

    for(int i = 0; i < list_len; i++) {
        sys_snode_t *slist_node = sys_slist_get(&slist);
        list_data_ptr = CONTAINER_OF(slist_node, struct slist_data_t, node);
        printf("Get list data: %d\n", list_data_ptr->data);
    }

    // --------------------------------------------------------------------
    // create another list and merge it with first

    // append data to list
    for(int i = 0; i < LIST_NODES-2; i++) {
        list_data[i].data = i;
        sys_slist_append( &slist, &(list_data[i].node) );
    }

    // create other linked list
    sys_slist_t slist_other;
    sys_slist_init(&slist_other);	

    // add data to other list
    list_data[LIST_NODES-2].data = 101;
    sys_slist_append( &slist_other, &(list_data[LIST_NODES-2].node) );
    
    list_data[LIST_NODES-1].data = 102;
    sys_slist_append( &slist_other, &(list_data[LIST_NODES-1].node) );

    // merge lists
    printk("Merge lists\n");
    sys_slist_merge_slist(&slist, &slist_other);

    // iterate through list and print nodes
    SYS_SLIST_FOR_EACH_CONTAINER(&slist, list_data_ptr, node) {
        printf("List data: %d\n", list_data_ptr->data);
    }


    // --------------------------------------------------------------------
    // iterate throught list using peek methods
    sys_snode_t *node = sys_slist_peek_head(&slist);
    while(node != NULL) {
        list_data_ptr = CONTAINER_OF(node, struct slist_data_t, node);
        printf("Peek list data: %d\n", list_data_ptr->data);
        node = sys_slist_peek_next(node);
    }



    return 0;
}
