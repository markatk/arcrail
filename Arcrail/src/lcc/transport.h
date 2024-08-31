#pragma once

#include "../../configuration.h"
#include "types.h"

void transport_init();

void transport_update();

void transport_reset();

// internally called by network layer
void transport_process_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data);

void transport_send(lcc_mti_t mti, uint8_t length, uint8_t *data);

void transport_invoke_producer(uint8_t input, uint8_t state);
