#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "prgn.h"

    #define LCC_TRANSFER_LAYER_STATE_INHIBITED 0
    #define LCC_TRANSFER_LAYER_STATE_PERMITTED 1

    #define LCC_CHECK_ID_ALIAS_STATE_OFF 0x0
    #define LCC_CHECK_ID_ALIAS_STATE_1 0x7
    #define LCC_CHECK_ID_ALIAS_STATE_2 0x6
    #define LCC_CHECK_ID_ALIAS_STATE_3 0x5
    #define LCC_CHECK_ID_ALIAS_STATE_4 0x4
    #define LCC_CHECK_ID_ALIAS_STATE_WAIT 0x3

    #define LCC_CAN_CONTROL_RESERVE_ID 0x0700
    #define LCC_CAN_CONTROL_ALIAS_MAP_DEFINITION 0x0701
    #define LCC_CAN_CONTROL_ALIAS_MAP_ENQUIRY 0x0702
    #define LCC_CAN_CONTROL_ALIAS_MAP_RESET 0x0703

uint8_t _transfer_layer_state;
uint8_t _check_id_alias_state;
uint16_t _node_id_alias;

uint8_t _send_can_control_message(uint16_t content_field, uint8_t length, uint8_t *data);
void _handle_can_control_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data);
#endif

void lcc_init() {
    lcc_reset();
}

void lcc_update() {
    if (_transfer_layer_state == LCC_TRANSFER_LAYER_STATE_INHIBITED) {
        // send check id messages
        if (_check_id_alias_state >= LCC_CHECK_ID_ALIAS_STATE_4 && _check_id_alias_state <= LCC_CHECK_ID_ALIAS_STATE_1) {
            uint16_t content_field = (uint16_t)_check_id_alias_state << 12;

            if (_check_id_alias_state == LCC_CHECK_ID_ALIAS_STATE_1) {
                content_field |= ((uint16_t)LCC_UNIQUE_IDENTIFIER[0]) << 4 | LCC_UNIQUE_IDENTIFIER[1] >> 4;
            } else if (_check_id_alias_state == LCC_CHECK_ID_ALIAS_STATE_2) {
                content_field |= ((uint16_t)(LCC_UNIQUE_IDENTIFIER[1] & 0x0F)) << 8 | LCC_UNIQUE_IDENTIFIER[2];
            } else if (_check_id_alias_state == LCC_CHECK_ID_ALIAS_STATE_3) {
                content_field |= ((uint16_t)LCC_UNIQUE_IDENTIFIER[3]) << 4 | LCC_UNIQUE_IDENTIFIER[4] >> 4;
            } else if (_check_id_alias_state == LCC_CHECK_ID_ALIAS_STATE_4) {
                content_field |= ((uint16_t)(LCC_UNIQUE_IDENTIFIER[4] & 0x0F)) << 8 | LCC_UNIQUE_IDENTIFIER[5];
            }

            if (_send_can_control_message(content_field, 0, 0) == CAN_OK) {
                _check_id_alias_state -= 1;
            }
        } else if (_check_id_alias_state == LCC_CHECK_ID_ALIAS_STATE_WAIT) {
            // TODO: Wait for 200ms timeout

            // node id alias is free thus reserve it
            if (_send_can_control_message(LCC_CAN_CONTROL_RESERVE_ID, 0, 0) == CAN_OK) {
                _check_id_alias_state = LCC_CHECK_ID_ALIAS_STATE_OFF;
            }
        } else if (_check_id_alias_state == LCC_CHECK_ID_ALIAS_STATE_OFF && _transfer_layer_state == LCC_TRANSFER_LAYER_STATE_INHIBITED) {
            // transmit to permitted state by sending an alias map definition can control message
            if (_send_can_control_message(LCC_CAN_CONTROL_ALIAS_MAP_DEFINITION, 6, (uint8_t *)LCC_UNIQUE_IDENTIFIER) == CAN_OK) {
                _transfer_layer_state = LCC_TRANSFER_LAYER_STATE_PERMITTED;

                Serial.print("Transfer layer: permitted, alias=");
                Serial.println(_node_id_alias, HEX);
            }
        }
    }
}

void lcc_reset() {
    _transfer_layer_state = LCC_TRANSFER_LAYER_STATE_INHIBITED;

    prgn_load_node_id((uint8_t *)LCC_UNIQUE_IDENTIFIER);

    _node_id_alias = prgn_get_alias();
    _check_id_alias_state = LCC_CHECK_ID_ALIAS_STATE_1;
}

__attribute__((weak)) void lcc_on_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
}

#ifdef USE_LCC
void can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data) {
    // msb must be set for lcc messages
    if ((identifier & 0x10000000) == 0) {
        return;
    }

    uint16_t content_field = (identifier & 0x07FFF000) >> 12;
    uint16_t source_nid = identifier & 0x0FFF;

    // handle can control or lcc messages
    if ((identifier & 0x08000000) == 0) {
        _handle_can_control_message(content_field, source_nid, length, data);
    } else {
        lcc_on_message(content_field, source_nid, length, data);
    }
}

uint8_t _send_can_control_message(uint16_t content_field, uint8_t length, uint8_t *data) {
    uint32_t identifier = 0x10000000 | ((uint32_t)content_field) << 12 | _node_id_alias;

    return can_send_message(identifier, length, data);
}

void _handle_can_control_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
    Serial.print("CAN Control: content_field=");
    Serial.print(content_field, HEX);
    Serial.print(", source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", data=");

    for (uint8_t i = 0; i < length; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }

    Serial.println();

    // if in check node id alias process and same the tested alias
    if (_check_id_alias_state != LCC_CHECK_ID_ALIAS_STATE_OFF && source_nid == _node_id_alias) {
        // restart check process with new alias
        _node_id_alias = prgn_get_alias();
        _check_id_alias_state = LCC_CHECK_ID_ALIAS_STATE_1;

        Serial.println("Check id collision");
    }

    if (_transfer_layer_state == LCC_TRANSFER_LAYER_STATE_PERMITTED && source_nid == _node_id_alias) {
        if (_send_can_control_message(LCC_CAN_CONTROL_RESERVE_ID, 0, 0) == CAN_OK) {
            Serial.println("Check id for own alias received");
        } else {
            Serial.println("Can busy");
        }
    }
}
#endif
