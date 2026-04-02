#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_led));

int main(void) {

    // check that LEDC initialized correctly
    if( !pwm_is_ready_dt(&pwm_led) ) {
        printk("LEDC is not ready\n");
        return 0;
    }

    // get period in nanoseconds
    uint32_t period_ns = pwm_led.period;
    uint32_t period_percent_ns = period_ns / 100;

    // fade led
    int percents = 0;
    while(true) {
        k_msleep(30);
        percents = (percents + 1) % 100;
        pwm_set_pulse_dt(&pwm_led, percents * period_percent_ns);
    }

	return 0;
}
