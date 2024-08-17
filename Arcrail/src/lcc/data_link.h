#pragma once

#include "../../configuration.h"
#include "../can/can.h"

#define DATA_LINK_STATE_INHIBITED 0
#define DATA_LINK_STATE_PERMITTED 1

#define DATA_LINK_OK CAN_OK
#define DATA_LINK_BUSY CAN_BUSY

void data_link_init();

void data_link_update();

void data_link_reset();

uint8_t data_link_get_state();

uint16_t data_link_get_alias();

uint8_t data_link_send(uint16_t mti, uint8_t length, uint8_t *data);

// callbacks
