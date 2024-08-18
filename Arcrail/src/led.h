#pragma once

#include "../configuration.h"

// TODO: Rename off to static or something as off seems like the led cannot be turned on
#define LED_MODE_OFF 0
#define LED_MODE_BLINK 1
#define LED_MODE_FLASH 2

#define LED_MAX_BLINK_COUNT 0x0F

void led_init();

void led_update();

#ifdef USE_LEDS
void led_set(uint8_t led, uint8_t value);

bool led_get(uint8_t led);

void led_toggle(uint8_t led);

void led_blink(uint8_t led);

void led_blink(uint8_t led, uint8_t count);

void led_flash(uint8_t led);

uint8_t led_get_mode(uint8_t led);

    #ifdef STATUS_LED
void status_led_set(uint8_t value);

bool status_led_get();

void status_led_blink();

void status_led_flash();

uint8_t status_led_get_mode();
    #endif
#endif
