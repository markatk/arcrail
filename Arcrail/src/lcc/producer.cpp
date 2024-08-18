#include "producer.h"

#ifdef USE_LCC
    #include "../settings.h"
    #include "callbacks.h"
    #include "events.h"
    #include "mti.h"
    #include "network.h"

    #ifdef USE_INPUTS
        #include "../inputs.h"

        // each input has two states (off and on)
        #define PRODUCER_COUNT INPUT_COUNT * 2
    #else
        #error Inputs must be used to use producers
    #endif

bool _producer_states[PRODUCER_COUNT]; // false = unadvertised

bool _try_get_first_unadvertised_producer_index(uint8_t *index);

    #ifdef USE_INPUTS
bool _try_get_input_event_id(uint8_t input, uint8_t state, uint8_t *event_id);
bool _is_input_advertised(uint8_t input, uint8_t state);
    #endif
#endif

void producer_init() {
}

void producer_update() {
#ifdef USE_LCC
    // do nothing until the network is initialized
    if (network_get_state() != NETWORK_STATE_INITIALIZED) {
        return;
    }

    // advertise first unadvertised producer
    uint8_t index;
    if (_try_get_first_unadvertised_producer_index(&index) == false) {
        return;
    }

    // get correct mti for producer
    #ifdef USE_INPUTS
    uint8_t input = index / 2;
    uint8_t input_state = index % 2;

    uint8_t state;
    if (inputs_try_get_state(input, &state) == false) {
        return;
    }

    uint16_t mti;
    if ((input_state != 0 && state) || (input_state == 0 && !state)) {
        mti = MTI_PRODUCER_IDENTIFIED_VALID;
    } else {
        mti = MTI_PRODUCER_IDENTIFIED_INVALID;
    }

    uint8_t event_id[8];

    if (_try_get_input_event_id(input, input_state, event_id) == false) {
        return;
    }
    #else
    uint16_t mti = MTI_PRODUCER_IDENTIFIED_UNKNOWN;

        #error Unknown event id for producer
    #endif

    network_send(mti, LCC_EVENT_ID_LENGTH, event_id);

    // // producer was advertised
    _producer_states[index] = true;
#endif
}

void producer_reset() {
#ifdef USE_LCC
    for (uint8_t i = 0; i < PRODUCER_COUNT; i++) {
        _producer_states[i] = false;
    }
#endif
}

#ifdef USE_LCC
bool producer_process_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data) {
    // all messages are expected to have an event id or event id range as data
    if (length < LCC_EVENT_ID_LENGTH) {
        return false;
    }

    switch (mti) {
        case MTI_IDENTIFY_PRODUCER:
            lcc_on_identifiy_producer(data);
            return true;

        case MTI_PRODUCER_IDENTIFIED_VALID:
            lcc_on_producer_identified(data, LCC_EVENT_STATE_VALID);
            return true;

        case MTI_PRODUCER_IDENTIFIED_INVALID:
            lcc_on_producer_identified(data, LCC_EVENT_STATE_INVALID);
            return false;

        case MTI_PRODUCER_IDENTIFIED_UNKNOWN:
            lcc_on_producer_identified(data, LCC_EVENT_STATE_UNKNOWN);
            return false;

        case MTI_PRODUCER_RANGE_IDENTIFIED:
            lcc_on_producer_range_identified(data);
            return false;

        default:
            return false;
    }
}

    #ifdef USE_INPUTS
void producer_process_input(uint8_t input, uint8_t state) {
    // if the producer is not advertised do not send anything
    if (_is_input_advertised(input, state) == false) {
        return;
    }

    uint8_t event_id[8];
    if (_try_get_input_event_id(input, state, event_id) == false) {
        return;
    }

    network_send(MTI_PRODUCER_CONSUMER_EVENT_REPORT, LCC_EVENT_ID_LENGTH, event_id);
}
    #endif

bool _try_get_first_unadvertised_producer_index(uint8_t *index) {
    for (uint8_t i = 0; i < PRODUCER_COUNT; i++) {
        if (_producer_states[i] == false) {
            *index = i;

            return true;
        }
    }

    return false;
}

    #ifdef USE_INPUTS
bool _try_get_input_event_id(uint8_t input, uint8_t state, uint8_t *event_id) {
    if (input >= INPUT_COUNT) {
        return false;
    }

    // copy first 6 bytes from the node id
    uint8_t *node_id = settings_get_lcc_node_id();

    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        event_id[i] = node_id[i];
    }

    // last 2 bytes are the event
    event_id[6] = LCC_EVENT_INPUT_HIGH_BYTE;
    event_id[7] = (input & 0x7F) << 1;

    if (state) {
        event_id[7] |= 0x01;
    }

    return true;
}

bool _is_input_advertised(uint8_t input, uint8_t state) {
    if (input >= INPUT_COUNT) {
        return false;
    }

    uint8_t index = input * 2;
    if (state) {
        index += 1;
    }

    return _producer_states[index];
}
    #endif
#endif
