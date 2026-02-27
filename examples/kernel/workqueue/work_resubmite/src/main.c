#include <stdio.h>
#include <zephyr/kernel.h>

void work_handler(struct k_work *work);

// define work items
K_WORK_DEFINE(work_item, work_handler);

// work item handler resubmit itself every second
void work_handler(struct k_work *work) {
    k_msleep(1000);
    printf("Work item resubmit\n");
    k_work_submit(work);
}

int main() {
    // submite work item to queue first time
    k_work_submit(&work_item);

    return 0;
}
