#include "network.h"

#ifdef USE_LCC
    #include "../settings.h"
    #include "callbacks.h"
    #include "data_link.h"
    #include "lcc.h"
    #include "mti.h"
    #include "transport.h"

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
        data_link_send(MTI_INITIALIZATION_COMPLETE_SIMPLE_SET, LCC_NODE_ID_LENGTH, settings_get_lcc_node_id().data);

        _state = NETWORK_STATE_INITIALIZED;
    }
}

void network_reset() {
    _state = NETWORK_STATE_UNINITIALIZED;
}

uint8_t network_get_state() {
    return _state;
}

void network_process_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data) {
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
    transport_process_message(mti, source_nid, length, data);

    lcc_node_id_t node_id;

    for (uint8_t i = 0; i < length && i < LCC_NODE_ID_LENGTH; i++) {
        node_id.data[i] = data[i];
    }

    if (verify_node_id) {
        lcc_on_verify_node_id(length, node_id);
    }

    switch (mti) {
        case MTI_INITIALIZATION_COMPLETE_FULL_PROTOCOL:
        case MTI_INITIALIZATION_COMPLETE_SIMPLE_SET:
            lcc_on_initialization_complete(source_nid, node_id);
            break;

        case MTI_VERIFIED_NODE_ID_FULL_PROTOCOL:
            lcc_on_verified_node_id(node_id, false);
            break;

        case MTI_VERIFIED_NODE_ID_SIMPLE_SET:
            lcc_on_verified_node_id(node_id, true);
            break;

        default:
            break;
    }
}

void network_send(lcc_mti_t mti, uint8_t length, uint8_t *data) {
    data_link_send(mti, length, data);
}

bool _compare_node_id(uint8_t *in) {
    lcc_node_id_t node_id = settings_get_lcc_node_id();

    for (uint8_t i = 0; i < LCC_NODE_ID_LENGTH; i++) {
        if (node_id.data[i] != in[i]) {
            return false;
        }
    }

    return true;
}

void _send_verified_node_id() {
    // TODO: Support full protocol
    data_link_send(MTI_VERIFIED_NODE_ID_SIMPLE_SET, LCC_NODE_ID_LENGTH, settings_get_lcc_node_id().data);
}
#endif
