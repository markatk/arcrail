#pragma once

#include <Arduino.h>

void inputs_init();

void inputs_update();

void inputs_reset();

#ifdef USE_INPUTS
bool inputs_try_get_state(uint8_t input, uint8_t *state);
#endif
