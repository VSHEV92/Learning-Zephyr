#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_dummy.h>

int main() {

    const struct shell *sh = shell_backend_dummy_get_ptr();
    
    printk("Bluetooth shell example\n");

    shell_execute_cmd(sh, "bt init");
    shell_execute_cmd(sh, "bt scan-filter-set name \"ESP32C6 Core Temp\"");
    shell_execute_cmd(sh, "bt scan-verbose-output on");
    shell_execute_cmd(sh, "bt scan passive dups");
    
    return 0;
}
