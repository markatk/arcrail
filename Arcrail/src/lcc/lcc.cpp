#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "callbacks.h"
    #include "data_link.h"
    #include "mti.h"
    #include "network.h"
#endif

void lcc_init() {
    data_link_init();
    network_init();

    lcc_reset();
}

void lcc_update() {
    data_link_update();
    network_update();
}

void lcc_reset() {
    data_link_reset();
    network_reset();
}

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
