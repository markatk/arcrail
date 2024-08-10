#pragma once

#include "../../configuration.h"

void lcc_init();

void lcc_update();

void lcc_reset();

void lcc_on_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data);
