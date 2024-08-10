#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"

    #define LCC_TRANSFER_LAYER_STATE_INHIBITED 0
    #define LCC_TRANSFER_LAYER_STATE_PERMITTED 1

uint8_t _transfer_layer_state;
uint16_t _node_id_alias;

void _handle_can_control_message(uint32_t identifier, uint8_t length, uint8_t *data);
#endif

void lcc_init() {
    lcc_reset();
}

void lcc_update() {
}

void lcc_reset() {
    _transfer_layer_state = LCC_TRANSFER_LAYER_STATE_INHIBITED;
    _node_id_alias = 0;
}

__attribute__((weak)) void lcc_on_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
}

#ifdef USE_LCC
void can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data) {
    // msb must be set for lcc messages
    if ((identifier & 0x10000000) == 0) {
        return;
    }

    // handle can control messages
    if ((identifier & 0x08000000) == 0) {
        _handle_can_control_message(identifier, length, data);

        return;
    }

    uint16_t content_field = (identifier & 0x07FFF000) >> 12;
    uint16_t source_nid = identifier & 0x0FFF;

    lcc_on_message(content_field, source_nid, length, data);
}

void _handle_can_control_message(uint32_t identifier, uint8_t length, uint8_t *data) {
}
#endif
