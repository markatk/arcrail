#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "../settings.h"
    #include "callbacks.h"
    #include "data_link.h"
    #include "events.h"
    #include "mti.h"
    #include "network.h"
    #include "producer_consumer.h"

    #ifdef LCC_USE_BLUE_GOLD
        #include "blue_gold.h"
    #endif

void _process_event_report(uint8_t length, uint8_t *payload);
#endif

void lcc_init() {
#ifdef USE_LCC
    data_link_init();
    network_init();
    producer_consumer_init();

    #ifdef LCC_USE_BLUE_GOLD
    blue_gold_init();
    #endif
#endif

    lcc_reset();
}

void lcc_update() {
#ifdef USE_LCC
    data_link_update();
    network_update();
    producer_consumer_update();

    #ifdef LCC_USE_BLUE_GOLD
    blue_gold_update();
    #endif
#endif
}

void lcc_reset() {
#ifdef USE_LCC
    data_link_reset();
    network_reset();
    producer_consumer_reset();

    #ifdef LCC_USE_BLUE_GOLD
    blue_gold_reset();
    #endif
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
    producer_consumer_process_message(mti, source_nid, length, data);

    switch (mti) {
        case MTI_PRODUCER_CONSUMER_EVENT_REPORT:
            _process_event_report(length, data);
            break;

        case MTI_PRODUCER_CONSUMER_EVENT_REPORT_WITH_PAYLOAD:
            _process_event_report(length, data);
            break;

        default:
            break;
    }

    // callbacks
    lcc_on_message(mti, source_nid, length, data);
}

    #ifdef USE_INPUTS
void lcc_invoke_producer(uint8_t input, uint8_t state) {
    producer_consumer_process_input(input, state);
}
    #endif

void _process_event_report(uint8_t length, uint8_t *payload) {
    if (length < LCC_EVENT_ID_LENGTH) {
        return;
    }

    lcc_event_id_t event_id;

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        event_id.data[i] = payload[i];
    }

    lcc_on_producer_consumer_event_report(event_id, length - LCC_EVENT_ID_LENGTH, payload + LCC_EVENT_ID_LENGTH);
}
#endif
