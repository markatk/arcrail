#include "data_link.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "../settings.h"
    #include "../timer.h"
    #include "callbacks.h"
    #include "network.h"
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
lcc_node_id_alias_t _node_id_alias;

void _send_can_control_message(uint16_t content_field, uint8_t length, uint8_t *data);
void _process_message(uint16_t content_field, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data);

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

        lcc_node_id_t node_id = settings_get_lcc_node_id();

        if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_1) {
            content_field |= ((uint16_t)node_id.data[0]) << 4 | node_id.data[1] >> 4;
        } else if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_2) {
            content_field |= ((uint16_t)(node_id.data[1] & 0x0F)) << 8 | node_id.data[2];
        } else if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_3) {
            content_field |= ((uint16_t)node_id.data[3]) << 4 | node_id.data[4] >> 4;
        } else if (_check_id_alias_state == CHECK_ID_ALIAS_STATE_4) {
            content_field |= ((uint16_t)(node_id.data[4] & 0x0F)) << 8 | node_id.data[5];
        }

        _send_can_control_message(content_field, 0, 0);

        _check_id_alias_state -= 1;
    } else if (_check_id_alias_state > 0) {
        // Wait at least for 200ms timeout
        if (_check_id_alias_state > 1 && timer_was_triggered()) {
            _check_id_alias_state -= 1;
        }

        // node id alias is free thus reserve it
        if (_check_id_alias_state == 1) {
            _send_can_control_message(CAN_CONTROL_RESERVE_ID, 0, 0);

            _check_id_alias_state = CHECK_ID_ALIAS_STATE_OFF;
        }
    } else {
        // transmit to permitted state by sending an alias map definition can control message
        _send_can_control_message(CAN_CONTROL_ALIAS_MAP_DEFINITION, LCC_NODE_ID_LENGTH, settings_get_lcc_node_id().data);

        _data_link_state = DATA_LINK_STATE_PERMITTED;
    }
}

void data_link_reset() {
    _data_link_state = DATA_LINK_STATE_INHIBITED;

    prgn_load_node_id(settings_get_lcc_node_id().data);

    _node_id_alias = prgn_get_alias();
    _check_id_alias_state = CHECK_ID_ALIAS_STATE_1;
}

uint8_t data_link_get_state() {
    return _data_link_state;
}

lcc_node_id_alias_t data_link_get_alias() {
    // do not return the alias if it is not reserved properly
    if (_data_link_state == DATA_LINK_STATE_INHIBITED) {
        return 0;
    }

    return _node_id_alias;
}

void data_link_send(lcc_mti_t mti, uint8_t length, uint8_t *data) {
    uint32_t identifier = 0x19000000 | ((uint32_t)mti & 0x0FFF) << 12 | _node_id_alias;

    can_send_message(identifier, length, data);
}

void can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data) {
    // msb must be set for lcc messages
    if ((identifier & 0x10000000) == 0) {
        return;
    }

    uint16_t content_field = (identifier & 0x07FFF000) >> 12;
    lcc_node_id_alias_t source_nid = identifier & 0x0FFF;

    // handle can control or lcc messages
    if ((identifier & 0x08000000) == 0) {
        _process_message(content_field, source_nid, length, data);
    } else if ((content_field & 0x7000) == 0x1000) {
        lcc_mti_t mti = content_field & 0x0FFF;

        network_process_message(mti, source_nid, length, data);
    }
}

void _send_can_control_message(uint16_t content_field, uint8_t length, uint8_t *data) {
    uint32_t identifier = 0x10000000 | ((uint32_t)content_field) << 12 | _node_id_alias;

    can_send_message(identifier, length, data);
}

void _process_message(uint16_t content_field, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data) {
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
        _send_can_control_message(CAN_CONTROL_RESERVE_ID, 0, 0);
    }

    // handle callbacks
    lcc_on_can_control_message(content_field, source_nid, length, data);

    lcc_node_id_t node_id;

    for (uint8_t i = 0; i < length && i < LCC_NODE_ID_LENGTH; i++) {
        node_id.data[i] = data[i];
    }

    switch (content_field) {
            // TODO: Handle check id messages

        case CAN_CONTROL_RESERVE_ID:
            lcc_on_reserve_id(source_nid);
            break;

        case CAN_CONTROL_ALIAS_MAP_DEFINITION:
            lcc_on_alias_map_definition(source_nid, node_id);
            break;

        case CAN_CONTROL_ALIAS_MAP_ENQUIRY:
            lcc_on_alias_map_enquiry(source_nid, node_id);
            break;

        case CAN_CONTROL_ALIAS_MAP_RESET:
            lcc_on_alias_map_reset(source_nid, node_id);
            break;

        default:
            break;
    }
}
#endif
