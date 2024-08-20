#pragma once

#include "../configuration.h"

#ifdef USE_LCC
    #include "lcc/types.h"
#endif

#define SWITCHING_MODE_OFF 0
#define SWITCHING_MODE_TIME 1
#define SWITCHING_MODE_BLINK 2
#define SWITCHING_MODE_RANDOM 3

void settings_init();

// TODO: Add settings_reset_to_firmware function

bool settings_set_value(uint16_t address, uint16_t value);

bool settings_get_value(uint16_t address, uint16_t *value);

bool settings_get_output_turn_on_address(uint8_t output, uint16_t *address, uint8_t *direction);

bool settings_get_output_turn_off_address(uint8_t output, uint16_t *address, uint8_t *direction);

bool settings_get_output_switching_mode(uint8_t output, uint8_t *mode, uint16_t *parameter);

bool settings_get_output_switching_2nd_parameter(uint8_t output, bool *enabled, uint16_t *parameter);

bool settings_get_output_delay(uint8_t output, uint16_t *delay);

bool settings_has_output_delay(uint8_t output);

bool settings_get_input_address(uint8_t input, uint16_t *address);

bool settings_get_input_delay(uint8_t input, uint16_t *delay);

#ifdef USE_LCC
bool settings_set_lcc_node_id(lcc_node_id_t node_id);

lcc_node_id_t settings_get_lcc_node_id();

lcc_event_id_t settings_get_lcc_next_event_id();

bool settings_set_lcc_producer_consumer_event_id(uint8_t producer_consumer, lcc_event_id_t event_id);

bool settings_get_lcc_producer_consumer_event_id(uint8_t producer_consumer, lcc_event_id_t event_id);
#endif

bool settings_on_programming_helper(uint8_t mode, uint16_t parameter);
