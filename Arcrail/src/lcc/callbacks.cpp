#include "callbacks.h"

__attribute__((weak)) void lcc_on_can_control_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
}

__attribute__((weak)) void lcc_on_check_id(uint16_t content_field, uint16_t source_nid) {
}

__attribute__((weak)) void lcc_on_reserve_id(uint16_t source_nid) {
}

__attribute__((weak)) void lcc_on_alias_map_definition(uint16_t source_nid, uint8_t *full_node_id) {
}

__attribute__((weak)) void lcc_on_alias_map_enquiry(uint16_t source_nid, uint8_t *full_node_id) {
}

__attribute__((weak)) void lcc_on_alias_map_reset(uint16_t source_nid, uint8_t *full_node_id) {
}

__attribute__((weak)) void lcc_on_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data) {
}

__attribute__((weak)) void lcc_on_initialization_complete(uint16_t source_nid, uint8_t *full_node_id) {
}

__attribute__((weak)) void lcc_on_verify_node_id(uint8_t length, uint8_t *full_node_id) {
}

__attribute__((weak)) void lcc_on_verified_node_id(uint8_t *full_node_id, bool simple_set) {
}

__attribute__((weak)) void lcc_on_producer_consumer_event_report(uint8_t *full_node_id, uint16_t event, uint8_t length, uint8_t *payload) {
}

__attribute__((weak)) void lcc_on_identifiy_producer(uint8_t *event_id) {
}

__attribute__((weak)) void lcc_on_producer_identified(uint8_t *event_id, uint8_t event_state) {
}

__attribute__((weak)) void lcc_on_producer_range_identified(uint8_t *event_id_range) {
}
