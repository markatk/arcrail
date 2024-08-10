#pragma once

#include "../configuration.h"

void buttons_init();

void buttons_update();

#ifdef USE_BUTTONS
bool button_is_just_pressed(uint8_t button);

bool button_is_pressed(uint8_t button);

bool button_is_just_released(uint8_t button);

bool button_is_released(uint8_t button);
#endif
