#include <led_strip.h>

#define THREAD_STACK_SIZE 1024 
#define THREAD_PRIORITY 1

#define HSL_FULL_PERIOD_MS 5000

#define RGB_LED_NODE DT_NODELABEL(rgb_led)

static const struct device *rgb_led_dev = DEVICE_DT_GET(RGB_LED_NODE);

char hue[4] = "0";
uint8_t hue_rotate = 0;

static void hsl_to_rgb(float h, float s, float l, struct led_rgb *rgb);
static void threads_entry(void * arg_1, void * arg_2, void * arg_3);


void led_strip_on() {
    struct led_rgb rgb_led_cfg;

    float hue = 0.0f;
    float saturation = 0.99f;
    float lightness = 0.50f;

    hsl_to_rgb(hue, saturation, lightness, &rgb_led_cfg);
    led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);
}


void led_strip_off() {
    struct led_rgb rgb_led_cfg = {0};

    led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);

    hue_rotate = 0;
}


void led_strip_update() {
    struct led_rgb rgb_led_cfg;

    int hue_value;
    float saturation = 0.99f;
    float lightness = 0.50f;

    sscanf(hue, "%d", &hue_value);

    hsl_to_rgb((float)hue_value, saturation, lightness, &rgb_led_cfg);
    led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);
}


static void threads_entry(void * arg_1, void * arg_2, void * arg_3) {
    struct led_rgb rgb_led_cfg = {0};

    led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);

    int hue_value;
    float saturation = 0.99f;
    float lightness = 0.50f;

    sscanf(hue, "%d", &hue_value);

    while(true) {
        k_msleep( (int)(HSL_FULL_PERIOD_MS / 360) );

        if((int)hue_rotate) {

            hsl_to_rgb((float)hue_value, saturation, lightness, &rgb_led_cfg);
            led_strip_update_rgb(rgb_led_dev, &rgb_led_cfg, 1);
        
            hue_value += 1;
            if (hue_value >= 360) hue_value = 0;
            
            snprintf(hue, 4, "%u", hue_value);
        }
    }
}

K_THREAD_DEFINE( 
    tid,                     /* thread id          */
    THREAD_STACK_SIZE,       /* stack size         */
    threads_entry,           /* thread entry point */
    NULL,                    /* argument 1         */ 
    NULL,                    /* argument 2         */
    NULL,                    /* argument 3         */
    THREAD_PRIORITY,         /* thread priority    */ 
    0,                       /* thread options     */
    0                        /* delay before start */
);

static void hsl_to_rgb(float h, float s, float l, struct led_rgb *rgb) {
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
