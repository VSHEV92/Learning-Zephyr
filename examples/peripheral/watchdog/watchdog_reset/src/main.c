#include <zephyr/kernel.h>
#include <zephyr/drivers/watchdog.h>


#define WATCHDOG_NODE DT_NODELABEL(wdt0)
static const struct device *wdt0_dev = DEVICE_DT_GET(WATCHDOG_NODE);

int main(void) {
    int wdt_chan_id;

    // watchdog configuration
    struct wdt_timeout_cfg wdt_cfg = {
        .window = {
            .min = 0,
            .max = 2000, // 2 sec
        },
        .callback = NULL,
        .flags = WDT_FLAG_RESET_SOC,
    };
    
    // setup timeout
    wdt_chan_id = wdt_install_timeout(wdt0_dev, &wdt_cfg);

    // start watchdog
    wdt_setup(wdt0_dev, WDT_OPT_PAUSE_HALTED_BY_DBG);

    k_msleep(1000);
    wdt_feed(wdt0_dev, wdt_chan_id);
    printk("Feed after 1 second\n");

    k_msleep(1000);
    wdt_feed(wdt0_dev, wdt_chan_id);
    printk("Feed after 1 second again and wait for reset\n");

	return 0;
}
