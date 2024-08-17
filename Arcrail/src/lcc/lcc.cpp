#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "../settings.h"
    #include "callbacks.h"
    #include "data_link.h"
    #include "events.h"
    #include "mti.h"
    #include "network.h"
#endif

void lcc_init() {
#ifdef USE_LCC
    data_link_init();
    network_init();
#endif

    lcc_reset();
}

void lcc_update() {
#ifdef USE_LCC
    data_link_update();
    network_update();
#endif
}

void lcc_reset() {
#ifdef USE_LCC
    data_link_reset();
    network_reset();
#endif
}

#ifdef USE_LCC
uint8_t lcc_get_state() {
    if (data_link_get_state() != DATA_LINK_STATE_PERMITTED) {
        return LCC_STATE_UNINITIALIZED;
    }

    if (network_get_state() != NETWORK_STATE_INITIALIZED) {
        return LCC_STATE_DATA_LINK_INITIALIZED;
    }

    return LCC_STATE_NETWORK_INITIALIZED;
}

uint8_t lcc_verify_node_id_addressed(uint8_t *node_id) {
    return data_link_send(MTI_VERIFY_NODE_ID_ADDRESSED, NODE_ID_LENGTH, node_id);
}

uint8_t lcc_verify_node_id_global() {
    return data_link_send(MTI_VERIFY_NODE_ID_GLOBAL, 0, 0);
}

    #ifdef USE_INPUTS
void lcc_invoke_producer(uint8_t input, uint8_t state) {
    // calculate event id
    uint8_t event_id[8];

    // first 6 bytes are the node id
    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        event_id[i] = settings_get_lcc_node_id()[i];
    }

    // last 2 bytes are the event
    event_id[6] = LCC_EVENT_INPUT_HIGH_BYTE;
    event_id[7] = (input & 0x7F) << 1;

    if (state) {
        event_id[7] |= 0x01;
    }

    data_link_send(MTI_PRODUCER_CONSUMER_EVENT_REPORT, LCC_EVENT_ID_LENGTH, event_id);
}
    #endif
#endif
