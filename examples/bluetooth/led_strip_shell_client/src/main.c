#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_dummy.h>

#define HSL_FULL_PERIOD_MS 5000

char cmd[100];

int main() {

    const struct shell *sh = shell_backend_dummy_get_ptr();
    
    printk("Bluetooth shell example\n");

    shell_execute_cmd(sh, "bt init");
    shell_execute_cmd(sh, "bt connect-name \"Esp32 Led Strip\"");

    uint32_t hue = 0;
    int temp, hundreds, decimals, ones;

    while(true) {
        k_msleep( (int)(HSL_FULL_PERIOD_MS / 360) );

        // convert integer to hex symbols
        temp = hue;

        hundreds = temp / 100 + '0';
        temp %= 100;

        decimals = temp / 10 + '0';
        temp %= 10;

        ones = temp + '0';

        // write to attribute
        sprintf(cmd, "gatt write b 0 %02x%02x%02x", hundreds, decimals, ones);
        shell_execute_cmd(sh, cmd);
        
        // increment hue
        hue++;
        if (hue == 360) hue = 0;
    }
    
    return 0;
}
