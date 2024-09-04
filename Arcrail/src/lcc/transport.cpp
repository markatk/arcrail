#include "transport.h"

#ifdef USE_LCC
    #include "callbacks.h"
    #include "lcc.h"
    #include "mti.h"
    #include "network.h"
    #include "producer_consumer.h"

    #ifdef LCC_USE_BLUE_GOLD
        #include "blue_gold.h"
    #endif

void _process_event_report(uint8_t length, uint8_t *payload);
void _process_learn_event(uint8_t length, uint8_t *payload);
#endif

void transport_init() {
    producer_consumer_init();

#ifdef LCC_USE_BLUE_GOLD
    blue_gold_init();
#endif
}

void transport_update() {
    producer_consumer_update();

#ifdef LCC_USE_BLUE_GOLD
    blue_gold_update();
#endif
}

void transport_reset() {
    producer_consumer_reset();

#ifdef LCC_USE_BLUE_GOLD
    blue_gold_reset();
#endif
}

void transport_process_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data) {
    producer_consumer_process_message(mti, source_nid, length, data);

    switch (mti) {
        case MTI_PRODUCER_CONSUMER_EVENT_REPORT:
            _process_event_report(length, data);
            break;

        case MTI_PRODUCER_CONSUMER_EVENT_REPORT_WITH_PAYLOAD:
            _process_event_report(length, data);
            break;

        case MTI_LEARN_EVENT:
            _process_learn_event(length, data);
            break;

        default:
            break;
    }

    // callbacks
    lcc_process_message(mti, source_nid, length, data);
}

void transport_send(lcc_mti_t mti, uint8_t length, uint8_t *data) {
    network_send(mti, length, data);
}

void transport_invoke_producer(uint8_t input, uint8_t state) {
    producer_consumer_process_input(input, state);
}

#ifdef USE_LCC
void _process_event_report(uint8_t length, uint8_t *payload) {
    if (length < LCC_EVENT_ID_LENGTH) {
        return;
    }

    lcc_event_id_t event_id;

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        event_id.data[i] = payload[i];
    }

    #ifdef USE_OUTPUTS
    producer_consumer_process_event_report(event_id);
    #endif

    lcc_on_producer_consumer_event_report(event_id, length - LCC_EVENT_ID_LENGTH, payload + LCC_EVENT_ID_LENGTH);
}

void _process_learn_event(uint8_t length, uint8_t *payload) {
    if (length < LCC_EVENT_ID_LENGTH) {
        return;
    }

    lcc_event_id_t event_id;

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        event_id.data[i] = payload[i];
    }

    blue_gold_learn(event_id);

    lcc_on_learn_event(event_id);
}
#endif
