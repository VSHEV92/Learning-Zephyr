#include <zephyr/kernel.h>
#include <zephyr/drivers/watchdog.h>


#define WATCHDOG_NODE DT_NODELABEL(wdt0)
static const struct device *wdt0_dev = DEVICE_DT_GET(WATCHDOG_NODE);

bool wdt_feed_flag = true;

void wdt_cb(const struct device *dev, int channel_id) {
    printk("WDT channel %d callback triggered\n", channel_id);
    wdt_feed_flag = true;
}


int main(void) {
    int wdt_chan_id;

    // watchdog configuration
    struct wdt_timeout_cfg wdt_cfg = {
        .window = {
            .min = 0,
            .max = 2000, // 2 sec
        },
        .callback = wdt_cb,
        .flags = WDT_FLAG_RESET_NONE,
    };
    
    // setup timeout
    wdt_chan_id = wdt_install_timeout(wdt0_dev, &wdt_cfg);

    // start watchdog
    wdt_setup(wdt0_dev, WDT_OPT_PAUSE_HALTED_BY_DBG);


    int feed_cnt = 0;
    while(true) {
        if (wdt_feed_flag) {
            wdt_feed(wdt0_dev, wdt_chan_id);
            printk("Feed watchdog\n");
        }
        
        feed_cnt++;
        if ( !(feed_cnt % 3) ) wdt_feed_flag = false;
        
        k_msleep(1000);
    }
	return 0;
}
