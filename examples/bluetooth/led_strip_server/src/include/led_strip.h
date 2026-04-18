#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <math.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>

extern char hue[];
extern uint8_t hue_rotate;

void led_strip_on();
void led_strip_off();
void led_strip_update();

#endif
