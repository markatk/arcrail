#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "../settings.h"
    #include "callbacks.h"
    #include "data_link.h"
    #include "events.h"
    #include "mti.h"
    #include "network.h"
    #include "transport.h"
#endif

void lcc_init() {
#ifdef USE_LCC
    data_link_init();
    network_init();
    transport_init();
#endif

    lcc_reset();
}

void lcc_update() {
#ifdef USE_LCC
    data_link_update();
    network_update();
    transport_update();
#endif
}

void lcc_reset() {
#ifdef USE_LCC
    data_link_reset();
    network_reset();
    transport_reset();
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

void lcc_verify_node_id_addressed(lcc_node_id_t node_id) {
    data_link_send(MTI_VERIFY_NODE_ID_ADDRESSED, LCC_NODE_ID_LENGTH, node_id.data);
}

void lcc_verify_node_id_global() {
    data_link_send(MTI_VERIFY_NODE_ID_GLOBAL, 0, 0);
}

void lcc_process_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data) {
    // callbacks
    lcc_on_message(mti, source_nid, length, data);
}

    #ifdef USE_INPUTS
void lcc_invoke_producer(uint8_t input, uint8_t state) {
    transport_invoke_producer(input, state);
}
    #endif
#endif
