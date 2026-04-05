#include <math.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>

#define HSL_FULL_PERIOD_MS 5000

#define RGB_LED_NODE DT_NODELABEL(rgb_led)

static const struct device *rgb_led_dev = DEVICE_DT_GET(RGB_LED_NODE);

void hsl_to_rgb(float h, float s, float l, struct led_rgb *rgb);

int main(void) {

    struct led_rgb rgb_led_cfg;

    float hue = 0.0f;
    float saturation = 0.99f;
    float lightness = 0.50f;

    while(true) {
        // convert HSL to RGB
        hsl_to_rgb(hue, saturation, lightness, &rgb_led_cfg);
        
        // setup RGB LED
        led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);
    
        k_msleep( (int)(HSL_FULL_PERIOD_MS / 360) );

        // update HUE value
        hue += 1;
        if (hue >= 360.0f) hue = 0.0f;

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
