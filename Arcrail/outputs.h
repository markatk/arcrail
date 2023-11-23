#pragma once

#include <Arduino.h>

void outputs_init();

void outputs_update();

void outputs_parse(uint16_t address, bool direction);
