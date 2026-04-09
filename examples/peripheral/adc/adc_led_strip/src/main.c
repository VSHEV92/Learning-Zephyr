#include <math.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/led_strip.h>

#define RGB_LED_NODE DT_NODELABEL(rgb_led)

static const struct device *rgb_led_dev = DEVICE_DT_GET(RGB_LED_NODE);
static const struct adc_dt_spec adc_spec = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

void hsl_to_rgb(float h, float s, float l, struct led_rgb *rgb);

int main(void) {

    // configure led strip
    struct led_rgb rgb_led_cfg;

    float hue = 0.0f;
    float saturation = 0.99f;
    float lightness = 0.50f;

    uint32_t adc_data;
    uint32_t adc_mask = (1 << adc_spec.resolution) - 1;

    // create sampling sequence and init buffer
    struct adc_sequence sequence = {
		.buffer = &adc_data,
		.buffer_size = sizeof(adc_data),
	};

    adc_sequence_init_dt(&adc_spec, &sequence);
    adc_channel_setup_dt(&adc_spec);

    while(true) {

        // read adc data
        adc_read_dt(&adc_spec, &sequence);
        adc_data &= adc_mask;
        
        // convert raw data to hue 0..360
        hue = (adc_data * 360.0) / (1 << adc_spec.resolution);

        // convert hue to rgb
        hsl_to_rgb(hue, saturation, lightness, &rgb_led_cfg);
        
        // drive led strip
        led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);
    }

	return 0;
}

// HSL to RGB converter
void hsl_to_rgb(float h, float s, float l, struct led_rgb *rgb) {
    float c = (1.0f - fabsf(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;
    float rt, gt, bt;

    if (h < 60)      { rt = c; gt = x; bt = 0; }
    else if (h < 120) { rt = x; gt = c; bt = 0; }
    else if (h < 180) { rt = 0; gt = c; bt = x; }
    else if (h < 240) { rt = 0; gt = x; bt = c; }
    else if (h < 300) { rt = x; gt = 0; bt = c; }
    else             { rt = c; gt = 0; bt = x; }

    rgb->r = (uint8_t)((rt + m) * 255);
    rgb->g = (uint8_t)((gt + m) * 255);
    rgb->b = (uint8_t)((bt + m) * 255);
}
