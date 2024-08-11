#pragma once

#include "../../configuration.h"

#define DATA_LINK_STATE_INHIBITED 0
#define DATA_LINK_STATE_PERMITTED 1

void data_link_init();

void data_link_update();

void data_link_reset();

uint8_t data_link_get_state();

uint16_t data_link_get_alias();

void data_link_process_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data);

// callbacks
