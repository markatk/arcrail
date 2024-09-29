#include "producer_consumer.h"

#ifdef USE_LCC
    #include "../settings.h"
    #include "callbacks.h"
    #include "events.h"
    #include "lcc.h"
    #include "mti.h"
    #include "network.h"
    #include "utility.h"

    #ifdef USE_OUTPUTS
        #include "../outputs.h"
    #endif

    #ifdef USE_INPUTS
        #include "../inputs.h"
    #endif

// holds producer and consumers (starting with consumers)
typedef struct {
    lcc_event_id_t event_id;
    bool advertised;
} producer_consumer_t;

producer_consumer_t _producer_consumers[LCC_PRODUCER_CONSUMER_COUNT];

bool _try_get_first_unadvertised_producer_consumer_index(uint8_t *index);
lcc_mti_t _get_producer_consumer_event_mti(uint8_t index);

    #ifdef USE_INPUTS
bool _get_input_producer_consumer_index(uint8_t input, uint8_t state, uint8_t *index);
bool _try_get_input_event_id(uint8_t input, uint8_t state, lcc_event_id_t *event_id);
    #endif
#endif

void producer_consumer_init() {
#ifdef USE_LCC
    // read initial event ids into producers/consumers
    for (uint8_t i = 0; i < LCC_PRODUCER_CONSUMER_COUNT; i++) {
        settings_get_lcc_producer_consumer_event_id(i, &_producer_consumers[i].event_id);
    }
#endif
}

void producer_consumer_update() {
#ifdef USE_LCC
    // do nothing until the network is initialized
    if (network_get_state() != NETWORK_STATE_INITIALIZED) {
        return;
    }

    // advertise first unadvertised producer
    uint8_t index;
    if (_try_get_first_unadvertised_producer_consumer_index(&index) == false) {
        return;
    }

    lcc_mti_t mti = _get_producer_consumer_event_mti(index);
    if (mti == 0) {
        return;
    }

    Serial.print("Advertise producer/consumer ");
    Serial.print(index, DEC);
    Serial.print(", ");

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        Serial.print(_producer_consumers[index].event_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", ");

    Serial.println(mti, HEX);

    network_send(mti, LCC_EVENT_ID_LENGTH, _producer_consumers[index].event_id.data);

    // // producer was advertised
    _producer_consumers[index].advertised = true;
#endif
}

void producer_consumer_reset() {
#ifdef USE_LCC
    for (uint8_t i = 0; i < LCC_PRODUCER_CONSUMER_COUNT; i++) {
        _producer_consumers[i].advertised = false;
    }
#endif
}

#ifdef USE_LCC
bool producer_consumer_process_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data) {
    // all messages are expected to have an event id or event id range as data
    if (length < LCC_EVENT_ID_LENGTH) {
        return false;
    }

    lcc_event_id_t event_id;

    for (uint8_t i = 0; i < length && i < LCC_EVENT_ID_LENGTH; i++) {
        event_id.data[i] = data[i];
    }

    switch (mti) {
        case MTI_IDENTIFY_PRODUCER:
            lcc_on_identify_producer(event_id);
            return true;

        case MTI_PRODUCER_IDENTIFIED_VALID:
            lcc_on_producer_identified(event_id, LCC_EVENT_STATE_VALID);
            return true;

        case MTI_PRODUCER_IDENTIFIED_INVALID:
            lcc_on_producer_identified(event_id, LCC_EVENT_STATE_INVALID);
            return false;

        case MTI_PRODUCER_IDENTIFIED_UNKNOWN:
            lcc_on_producer_identified(event_id, LCC_EVENT_STATE_UNKNOWN);
            return false;

        case MTI_PRODUCER_RANGE_IDENTIFIED:
            lcc_on_producer_range_identified(event_id);
            return false;

        default:
            return false;
    }
}

bool producer_consumer_set_event_id(uint8_t producer_consumer, lcc_event_id_t event_id) {
    if (producer_consumer >= LCC_PRODUCER_CONSUMER_COUNT) {
        return false;
    }

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        _producer_consumers[producer_consumer].event_id.data[i] = event_id.data[i];
    }

    return settings_set_lcc_producer_consumer_event_id(producer_consumer, event_id);
}

bool producer_consumer_get_event_id(uint8_t producer_consumer, lcc_event_id_t *event_id) {
    if (producer_consumer >= LCC_PRODUCER_CONSUMER_COUNT) {
        return false;
    }

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        event_id->data[i] = _producer_consumers[producer_consumer].event_id.data[i];
    }

    return true;
}

    #ifdef USE_OUTPUTS
void producer_consumer_process_event_report(lcc_event_id_t event_id) {
    // process all producers with the given event
    for (uint8_t i = 0; i < LCC_CONSUMER_COUNT; i++) {
        if (compare_event_ids(_producer_consumers[i].event_id, event_id)) {
            uint8_t output = i / 2;
            uint8_t output_state = i % 2;

            outputs_set(output, output_state);
        }
    }
}
    #endif

    #ifdef USE_INPUTS
void producer_consumer_process_input(uint8_t input, uint8_t state) {
    lcc_event_id_t event_id;
    if (_try_get_input_event_id(input, state, &event_id) == false) {
        return;
    }

    network_send(MTI_PRODUCER_CONSUMER_EVENT_REPORT, LCC_EVENT_ID_LENGTH, event_id.data);
}
    #endif

bool _try_get_first_unadvertised_producer_consumer_index(uint8_t *index) {
    for (uint8_t i = 0; i < LCC_PRODUCER_CONSUMER_COUNT; i++) {
        if (_producer_consumers[i].advertised == false) {
            *index = i;

            return true;
        }
    }

    return false;
}

lcc_mti_t _get_producer_consumer_event_mti(uint8_t index) {
    #ifdef USE_OUTPUTS
    if (index < OUTPUT_COUNT * 2) {
        // handle output
        uint8_t output = index / 2;
        uint8_t output_state = index % 2;

        uint8_t state;
        if (outputs_try_get_state(output, &state) == false) {
            return MTI_CONSUMER_IDENTIFIED_UNKNOWN;
        }

        if ((output_state != 0 && state) || (output_state == 0 && !state)) {
            return MTI_CONSUMER_IDENTIFIED_VALID;
        } else {
            return MTI_CONSUMER_IDENTIFIED_INVALID;
        }
    } else {
    #elif defined(USE_INPUTS)
    if (index < INPUT_COUNT * 2) {
    #endif
    // handle input
    #ifdef USE_OUTPUTS
        uint8_t input = (index - OUTPUT_COUNT * 2) / 2;
    #else
        uint8_t input = index / 2;
    #endif

        uint8_t input_state = index % 2;
        uint8_t state;

        if (inputs_try_get_state(input, &state) == false) {
            return MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }

        if ((input_state != 0 && state) || (input_state == 0 && !state)) {
            return MTI_PRODUCER_IDENTIFIED_VALID;
        } else {
            return MTI_PRODUCER_IDENTIFIED_INVALID;
        }
    }

    return 0;
}

    #ifdef USE_INPUTS
bool _get_input_producer_consumer_index(uint8_t input, uint8_t state, uint8_t *index) {
    if (input >= INPUT_COUNT) {
        return false;
    }

        #ifdef USE_OUTPUTS
    *index = OUTPUT_COUNT * 2 + input * 2;
        #else
    *index = input * 2;
        #endif

    if (state) {
        *index += 1;
    }

    return true;
}

bool _try_get_input_event_id(uint8_t input, uint8_t state, lcc_event_id_t *event_id) {
    uint8_t index;
    if (_get_input_producer_consumer_index(input, state, &index) == false) {
        return false;
    }

    // if the producer/consumer is not advertised yet do not send anything
    if (_producer_consumers[index].advertised == false) {
        return false;
    }

    // copy event from producer/consumer
    *event_id = _producer_consumers[index].event_id;

    return true;
}
    #endif
#endif
