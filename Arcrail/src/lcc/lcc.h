#pragma once

#include "../../configuration.h"
#include "callbacks.h"
#include "types.h"

#define LCC_STATE_UNINITIALIZED 0
#define LCC_STATE_DATA_LINK_INITIALIZED 1
#define LCC_STATE_NETWORK_INITIALIZED 2

#if defined(USE_OUTPUTS) && defined(USE_INPUTS)
    #define LCC_PRODUCER_CONSUMER_COUNT (OUTPUT_COUNT * 2 + INPUT_COUNT * 2)
#elif defined(USE_OUTPUTS)
    #define LCC_PRODUCER_CONSUMER_COUNT (OUTPUT_COUNT * 2)
#elif defined(USE_INPUTS)
    #define LCC_PRODUCER_CONSUMER_COUNT (INPUT_COUNT * 2)
#else
    #define LCC_PRODUCER_CONSUMER_COUNT 0
#endif

void lcc_init();

void lcc_update();

void lcc_reset();

#ifdef USE_LCC
uint8_t lcc_get_state();

void lcc_verify_node_id_addressed(lcc_node_id_t node_id);

void lcc_verify_node_id_global();

// internally called by transport layer
void lcc_process_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data);

    #ifdef USE_INPUTS
void lcc_invoke_producer(uint8_t input, uint8_t state);
    #endif
#endif
