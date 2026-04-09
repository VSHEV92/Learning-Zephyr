#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>

static const struct adc_dt_spec adc_spec = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

int main(void) {

    uint32_t adc_data;
    uint32_t adc_mask = (1 << adc_spec.resolution) - 1;

    // create sampling sequence and init buffer
    struct adc_sequence sequence = {
		.buffer = &adc_data,
		.buffer_size = sizeof(adc_data),
	};

    // config sequence from adc spec 
    // Set channel, resolution, oversampling
    adc_sequence_init_dt(&adc_spec, &sequence);

    // setup adc channel from adc spec
    // we need to do this before first read_dt
    adc_channel_setup_dt(&adc_spec);

    while(true) {
        adc_read_dt(&adc_spec, &sequence);
    

        adc_data &= adc_mask;

        printk("ADC Value: %u\n", adc_data);

        k_msleep(500);
    }

	return 0;
}
