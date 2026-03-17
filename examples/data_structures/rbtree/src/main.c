#include <zephyr/kernel.h>
#include <zephyr/sys/rb.h>

#define MAX_TREE_NODES 10

struct tree_data {
    int data;
    struct rbnode node; 
};

// nodes compare function
static bool rb_cmp(struct rbnode *a, struct rbnode *b)
{
    struct tree_data *entry_a = CONTAINER_OF(a, struct tree_data, node);
    struct tree_data *entry_b = CONTAINER_OF(b, struct tree_data, node);
    return entry_a->data < entry_b->data;
}

// rb visit callback
void visit_cb(struct rbnode *node, void *cookie) {
    struct tree_data *entry = CONTAINER_OF(node, struct tree_data, node);
    printk("Visited entry data: %d\n", entry->data);
}

int main() {

    // create data for tree
    struct tree_data entry[MAX_TREE_NODES];
    struct tree_data *tree_entry;
    struct rbnode *node;

    for(int i = 0; i < MAX_TREE_NODES; i++) {
        entry[i].data = i;
    }

    // create red/black tree
    struct rbtree rbt = {
        .lessthan_fn = rb_cmp,
        .root = NULL
    };

    // insert nodes
    printk("Insert all nodes\n");
    for(int i = 0; i < MAX_TREE_NODES; i++) {
        rb_insert( &rbt, &(entry[i].node) );
    }

    // iterate throught tree nodes
    RB_FOR_EACH_CONTAINER(&rbt, tree_entry, node) {
        printk("Entry data: %d\n", tree_entry->data);
    }

    // remove some node
    printk("Remove some nodes\n");
    rb_remove(&rbt, &(entry[1].node));
    rb_remove(&rbt, &(entry[2].node));
    rb_remove(&rbt, &(entry[7].node));
    rb_remove(&rbt, &(entry[9].node));

    // iterate throught tree nodes after remove
    RB_FOR_EACH_CONTAINER(&rbt, tree_entry, node) {
        printk("Entry data: %d\n", tree_entry->data);
    }
    
    // get min and max values
    node = rb_get_min(&rbt);
    tree_entry = CONTAINER_OF(node, struct tree_data, node);
    printk("Tree min data: %d\n", tree_entry->data);

    node = rb_get_max(&rbt);
    tree_entry = CONTAINER_OF(node, struct tree_data, node);
    printk("Tree max data: %d\n", tree_entry->data);

    // check which node inside tree
    for(int i = 0; i < MAX_TREE_NODES; i++) {
        bool inside = rb_contains( &rbt, &(entry[i].node) );
        printk("Node %d %s inside tree\n", i, inside ? "" : "not");
    }

    // walk throught each node
    rb_walk(&rbt, visit_cb, NULL);


    return 0;

}
