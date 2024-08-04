#pragma once

#include <Arduino.h>

void loconet_init();

void loconet_update();

void loconet_report_sensor(uint16_t address, uint8_t state);
