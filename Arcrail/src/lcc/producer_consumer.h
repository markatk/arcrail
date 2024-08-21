#pragma once

#include "../../configuration.h"

void producer_consumer_init();

void producer_consumer_update();

void producer_consumer_reset();

#ifdef USE_LCC
bool producer_consumer_process_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data);

    #ifdef USE_INPUTS
void producer_consumer_process_input(uint8_t input, uint8_t state);
    #endif
#endif
