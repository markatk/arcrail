#pragma once

#include "../configuration.h"

void loconet_init();

void loconet_update();

#ifdef USE_LOCONET
void loconet_report_sensor(uint16_t address, uint8_t state);
#endif
