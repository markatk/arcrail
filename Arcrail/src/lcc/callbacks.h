#pragma once

#include "../../configuration.h"
#include "types.h"

#define LCC_EVENT_STATE_UNKNOWN 0
#define LCC_EVENT_STATE_VALID 1
#define LCC_EVENT_STATE_INVALID 2

// data link layer
void lcc_on_can_control_message(uint16_t content_field, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data);

void lcc_on_check_id(uint16_t content_field, lcc_node_id_alias_t source_nid);

void lcc_on_reserve_id(lcc_node_id_alias_t source_nid);

void lcc_on_alias_map_definition(lcc_node_id_alias_t source_nid, lcc_node_id_t node_id);

void lcc_on_alias_map_enquiry(lcc_node_id_alias_t source_nid, lcc_node_id_t node_id);

void lcc_on_alias_map_reset(lcc_node_id_alias_t source_nid, lcc_node_id_t node_id);

// network layer
void lcc_on_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data);

void lcc_on_initialization_complete(lcc_node_id_alias_t source_nid, lcc_node_id_t node_id);

// TODO: Split into two methods, one without payload (node_id)?
void lcc_on_verify_node_id(uint8_t length, lcc_node_id_t node_id);

void lcc_on_verified_node_id(lcc_node_id_t node_id, bool simple_set);

// transport layer
void lcc_on_producer_consumer_event_report(lcc_event_id_t event_id, uint8_t length, uint8_t *payload);

void lcc_on_identify_consumer(lcc_event_id_t event_id);

void lcc_on_consumer_identified(lcc_event_id_t, uint8_t event_state);

void lcc_on_consumer_range_identified(lcc_event_id_t event_id_range);

void lcc_on_identify_producer(lcc_event_id_t event_id);

void lcc_on_producer_identified(lcc_event_id_t event_id, uint8_t event_state);

void lcc_on_producer_range_identified(lcc_event_id_t event_id_range);

void lcc_on_learn_event(lcc_event_id_t event_id);
