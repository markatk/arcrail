#pragma once

#include "configuration.h"

void led_init();

void led_update();

#ifdef STATUS_LED
void status_led_set(uint8_t value);

void status_led_blink();
#endif
