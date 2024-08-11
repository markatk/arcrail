#pragma once

#include "../../configuration.h"
#include "callbacks.h"

#define LCC_STATE_UNINITIALIZED 0
#define LCC_STATE_DATA_LINK_INITIALIZED 1
#define LCC_STATE_NETWORK_INITIALIZED 2

void lcc_init();

void lcc_update();

void lcc_reset();

#ifdef USE_LCC
uint8_t lcc_get_state();

uint8_t lcc_verify_node_id_addressed(uint8_t *node_id);

uint8_t lcc_verify_node_id_global();
#endif
