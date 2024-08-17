#pragma once

#include "../../configuration.h"

#define NETWORK_STATE_UNINITIALIZED 0
#define NETWORK_STATE_INITIALIZED 1

void network_init();

void network_update();

void network_reset();

uint8_t network_get_state();

// internally called by data link layer
void network_process_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data);

uint8_t network_send(uint16_t mti, uint8_t length, uint8_t *data);
