#pragma once

#include <Arduino.h>

#define SWITCHING_MODE_OFF 0
#define SWITCHING_MODE_TIME 1
#define SWITCHING_MODE_BLINK 2
#define SWITCHING_MODE_RANDOM 3

void settings_init();

bool settings_set_value(uint16_t address, uint16_t value);

bool settings_get_value(uint16_t address, uint16_t *value);

bool settings_get_output_turn_on_address(uint8_t output, uint16_t *address, uint8_t *direction);

bool settings_get_output_turn_off_address(uint8_t output, uint16_t *address, uint8_t *direction);

bool settings_get_output_switching_mode(uint8_t output, uint8_t *mode, uint16_t *parameter);

bool settings_get_output_switching_2nd_parameter(uint8_t output, bool *enabled, uint16_t *parameter);

bool settings_get_output_delay(uint8_t output, uint16_t *delay);

bool settings_has_output_delay(uint8_t output);
