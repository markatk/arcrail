#include "network.h"

#ifdef USE_LCC
    #include "callbacks.h"
    #include "data_link.h"
    #include "mti.h"
uint8_t _state;

bool _compare_node_id(uint8_t *in);
void _send_verified_node_id();

void network_init() {
}

void network_update() {
    // nothing to do if state is initialized
    if (_state == NETWORK_STATE_INITIALIZED) {
        return;
    }

    // switch to initialized state
    if (data_link_get_state() == DATA_LINK_STATE_PERMITTED) {
        if (data_link_send(MTI_INITIALIZATION_COMPLETE_SIMPLE_SET, NODE_ID_LENGTH, (uint8_t *)LCC_UNIQUE_IDENTIFIER) == DATA_LINK_OK) {
            _state = NETWORK_STATE_INITIALIZED;
        }
    }
}

void network_reset() {
    _state = NETWORK_STATE_UNINITIALIZED;
}

uint8_t network_get_state() {
    return _state;
}

void network_process_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data) {
    // respond to verify node id messages
    bool verify_node_id = false;

    if (mti == MTI_VERIFY_NODE_ID_ADDRESSED || mti == MTI_VERIFY_NODE_ID_GLOBAL) {
        bool node_id_matches = _compare_node_id(data);

        if ((mti == MTI_VERIFY_NODE_ID_ADDRESSED && node_id_matches) || (mti == MTI_VERIFY_NODE_ID_GLOBAL && (length == 0 || node_id_matches))) {
            _send_verified_node_id();

            verify_node_id = true;
        }
    }

    // callbacks
    lcc_on_message(mti, source_nid, length, data);

    if (verify_node_id) {
        lcc_on_verify_node_id(length, data);
    }

    switch (mti) {
        case MTI_INITIALIZATION_COMPLETE_FULL_PROTOCOL:
        case MTI_INITIALIZATION_COMPLETE_SIMPLE_SET:
            lcc_on_initialization_complete(source_nid, data);
            break;

        case MTI_VERIFIED_NODE_ID_FULL_PROTOCOL:
            lcc_on_verified_node_id(data, false);
            break;

        case MTI_VERIFIED_NODE_ID_SIMPLE_SET:
            lcc_on_verified_node_id(data, true);
            break;

        default:
            break;
    }
}

bool _compare_node_id(uint8_t *in) {
    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        if (LCC_UNIQUE_IDENTIFIER[i] != in[i]) {
            return false;
        }
    }

    return true;
}

void _send_verified_node_id() {
    // TODO: Ensure message is send
    // TODO: Support full protocol
    data_link_send(MTI_VERIFIED_NODE_ID_SIMPLE_SET, NODE_ID_LENGTH, (uint8_t *)LCC_UNIQUE_IDENTIFIER);
}
#endif
