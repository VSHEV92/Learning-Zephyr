#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_dummy.h>

int main() {

    const struct shell *sh = shell_backend_dummy_get_ptr();
    
    printk("Bluetooth shell example\n");

    shell_execute_cmd(sh, "bt init");
    shell_execute_cmd(sh, "bt connect-name \"ESP32C6 Core Temp\"");

    while(true) {
        shell_execute_cmd(sh, "gatt read b");
        k_msleep(1000);
    }
    
    return 0;
}
