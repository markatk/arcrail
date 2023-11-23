#pragma once

#include "configuration.h"

void led_init();

#ifdef STATUS_LED
void status_led_set(uint8_t value);
#endif
