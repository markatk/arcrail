#pragma once

#include "../../configuration.h"
#include "types.h"

void producer_consumer_init();

void producer_consumer_update();

void producer_consumer_reset();

#ifdef USE_LCC
bool producer_consumer_process_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data);

bool producer_consumer_set_event_id(uint8_t producer_consumer, lcc_event_id_t event_id);

bool producer_consumer_get_event_id(uint8_t producer_consumer, lcc_event_id_t *event_id);

    #ifdef USE_OUTPUTS
void producer_consumer_process_event_report(lcc_event_id_t event_id);
    #endif

    #ifdef USE_INPUTS
void producer_consumer_process_input(uint8_t input, uint8_t state);
    #endif
#endif
