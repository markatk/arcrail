#pragma once

#include <Arduino.h>

void outputs_init();

void outputs_update();

void outputs_set(uint8_t output, uint8_t state);

void outputs_parse(uint16_t address, bool direction);

bool outputs_try_get_state(uint8_t output, uint8_t *state);
