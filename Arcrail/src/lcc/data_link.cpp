#include "data_link.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "../timer.h"
    #include "callbacks.h"
    #include "prgn.h"

    #define CHECK_ID_ALIAS_STATE_OFF 0x0
    #define CHECK_ID_ALIAS_STATE_1 0x7
    #define CHECK_ID_ALIAS_STATE_2 0x6
    #define CHECK_ID_ALIAS_STATE_3 0x5
    #define CHECK_ID_ALIAS_STATE_4 0x4

    #define CAN_CONTROL_RESERVE_ID 0x0700
    #define CAN_CONTROL_ALIAS_MAP_DEFINITION 0x0701
    #define CAN_CONTROL_ALIAS_MAP_ENQUIRY 0x0702
    #define CAN_CONTROL_ALIAS_MAP_RESET 0x0703

uint8_t _data_link_state;
uint8_t _check_id_alias_state;
uint16_t _node_id_alias;

uint8_t _send_can_control_message(uint16_t content_field, uint8_t length, uint8_t *data);

void data_link_init() {
}

void data_link_update() {
    // nothing needs to be done if the data link is established
    if (_data_link_state == DATA_LINK_STATE_PERMITTED) {
        return;
    }

    // send check id messages
    if (_check_id_alias_state >= CHECK_ID_ALIAS_STATE_4 && _check_id_alias_state <= CHECK_ID_ALIAS_STATE_1) {
        uint16_t content_field = (uint16_t)_check_id_alias_state << 12;

        if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_1) {
            content_field |= ((uint16_t)LCC_UNIQUE_IDENTIFIER[0]) << 4 | LCC_UNIQUE_IDENTIFIER[1] >> 4;
        } else if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_2) {
            content_field |= ((uint16_t)(LCC_UNIQUE_IDENTIFIER[1] & 0x0F)) << 8 | LCC_UNIQUE_IDENTIFIER[2];
        } else if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_3) {
            content_field |= ((uint16_t)LCC_UNIQUE_IDENTIFIER[3]) << 4 | LCC_UNIQUE_IDENTIFIER[4] >> 4;
        } else if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_4) {
            content_field |= ((uint16_t)(LCC_UNIQUE_IDENTIFIER[4] & 0x0F)) << 8 | LCC_UNIQUE_IDENTIFIER[5];
        }

        if (_send_can_control_message(content_field, 0, 0) == CAN_OK) {
            _check_id_alias_state -= 1;
        }
    } else if (_check_id_alias_state > 0) {
        // Wait at least for 200ms timeout
        if (_check_id_alias_state > 1 && timer_was_triggered()) {
            _check_id_alias_state -= 1;
        }

        // node id alias is free thus reserve it
        if (_check_id_alias_state == 1 && _send_can_control_message(CAN_CONTROL_RESERVE_ID, 0, 0) == CAN_OK) {
            _check_id_alias_state = CHECK_ID_ALIAS_STATE_OFF;
        }
    } else {
        // transmit to permitted state by sending an alias map definition can control message
        if (_send_can_control_message(CAN_CONTROL_ALIAS_MAP_DEFINITION, 6, (uint8_t *)LCC_UNIQUE_IDENTIFIER) == CAN_OK) {
            _data_link_state = DATA_LINK_STATE_PERMITTED;
        }
    }
}

void data_link_reset() {
    _data_link_state = DATA_LINK_STATE_INHIBITED;

    prgn_load_node_id((uint8_t *)LCC_UNIQUE_IDENTIFIER);

    _node_id_alias = prgn_get_alias();
    _check_id_alias_state = CHECK_ID_ALIAS_STATE_1;
}

uint8_t data_link_get_state() {
    return _data_link_state;
}

uint16_t data_link_get_alias() {
    // do not return the alias if it is not reserved properly
    if (_data_link_state == DATA_LINK_STATE_INHIBITED) {
        return 0;
    }

    return _node_id_alias;
}

void data_link_process_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
    // if in check node id alias process and same the tested alias
    if (_check_id_alias_state != CHECK_ID_ALIAS_STATE_OFF && source_nid == _node_id_alias) {
        // restart check process with new alias
        _node_id_alias = prgn_get_alias();
        _check_id_alias_state = CHECK_ID_ALIAS_STATE_1;
    }

    if (_data_link_state != DATA_LINK_STATE_PERMITTED) {
        return;
    }

    if (source_nid == _node_id_alias) {
        // TODO: ensure message is send
        _send_can_control_message(CAN_CONTROL_RESERVE_ID, 0, 0);
    }

    // handle callbacks
    lcc_on_can_control_message(content_field, source_nid, length, data);

    switch (content_field) {
            // TODO: Handle check id messages

        case CAN_CONTROL_RESERVE_ID:
            lcc_on_reserve_id(source_nid);
            break;

        case CAN_CONTROL_ALIAS_MAP_DEFINITION:
            lcc_on_alias_map_definition(source_nid, data);
            break;

        case CAN_CONTROL_ALIAS_MAP_ENQUIRY:
            lcc_on_alias_map_enquiry(source_nid, data);
            break;

        case CAN_CONTROL_ALIAS_MAP_RESET:
            lcc_on_alias_map_reset(source_nid, data);
            break;

        default:
            break;
    }
}

uint8_t _send_can_control_message(uint16_t content_field, uint8_t length, uint8_t *data) {
    uint32_t identifier = 0x10000000 | ((uint32_t)content_field) << 12 | _node_id_alias;

    return can_send_message(identifier, length, data);
}
#endif
