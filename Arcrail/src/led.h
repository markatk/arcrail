#pragma once

#include "../configuration.h"

void led_init();

void led_update();

#ifdef USE_LEDS
void led_set(uint8_t led, uint8_t value);

void led_toggle(uint8_t led);

void led_blink(uint8_t led);

void led_flash(uint8_t led);

    #ifdef STATUS_LED
void status_led_set(uint8_t value);

void status_led_blink();
    #endif
#endif
