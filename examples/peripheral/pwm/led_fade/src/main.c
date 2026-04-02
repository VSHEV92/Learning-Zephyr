#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

#define LEDC0_DEVICE DT_NODELABEL(ledc0)

static const struct device *ledc0_dev = DEVICE_DT_GET(LEDC0_DEVICE);

int main(void) {
    int percents = 0;

    // set pwm configuration
    pwm_set (
        ledc0_dev,            // devcie
        0,                    // channel
        PWM_USEC(100),        // period
        PWM_USEC(percents),   // pulse
        PWM_POLARITY_INVERTED // flags
    );

    // fade led
    while(true) {
        k_msleep(20);
        percents = (percents + 1) % 100;
    
        pwm_set (
            ledc0_dev,            // devcie
            0,                    // channel
            PWM_USEC(100),        // period
            PWM_USEC(percents),   // pulse
            PWM_POLARITY_INVERTED // flags
        );
    }

	return 0;
}
