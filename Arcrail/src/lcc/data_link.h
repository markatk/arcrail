#pragma once

#include "../../configuration.h"
#include "types.h"

#define DATA_LINK_STATE_INHIBITED 0
#define DATA_LINK_STATE_PERMITTED 1

void data_link_init();

void data_link_update();

void data_link_reset();

uint8_t data_link_get_state();

lcc_node_id_alias_t data_link_get_alias();

void data_link_send(lcc_mti_t mti, uint8_t length, uint8_t *data);

// callbacks
