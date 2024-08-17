#pragma once

#include "../../configuration.h"

void producer_init();

void producer_update();

void producer_reset();

#ifdef USE_LCC
    #ifdef USE_INPUTS
void producer_process_input(uint8_t input, uint8_t state);
    #endif
#endif
