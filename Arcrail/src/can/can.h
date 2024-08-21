#pragma once

#include "../../configuration.h"

#define CAN_OK 0x00
#define CAN_BUSY 0x01

void can_init();

void can_update();

void can_send_message(uint32_t identifier, uint8_t length, uint8_t *data);

bool can_is_transmit_buffer_full();

void can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data);
