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

__attribute__((weak)) void lcc_on_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
}
