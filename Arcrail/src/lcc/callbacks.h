#pragma once

#include "../../configuration.h"

// TODO: Rename to LCC_NODE_ID_LENGTH
#define NODE_ID_LENGTH 6
#define LCC_EVENT_ID_LENGTH 8

// data link layer
void lcc_on_can_control_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data);

void lcc_on_check_id(uint16_t content_field, uint16_t source_nid);

void lcc_on_reserve_id(uint16_t source_nid);

void lcc_on_alias_map_definition(uint16_t source_nid, uint8_t *full_node_id);

void lcc_on_alias_map_enquiry(uint16_t source_nid, uint8_t *full_node_id);

void lcc_on_alias_map_reset(uint16_t source_nid, uint8_t *full_node_id);

// network layer
void lcc_on_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data);

void lcc_on_initialization_complete(uint16_t source_nid, uint8_t *full_node_id);

void lcc_on_verify_node_id(uint8_t length, uint8_t *full_node_id);

void lcc_on_verified_node_id(uint8_t *full_node_id, bool simple_set);
