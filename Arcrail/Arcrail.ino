#include "configuration.h"

#include "src/buttons.h"
#include "src/can/can.h"
#include "src/inputs.h"
#include "src/lcc/lcc.h"
#include "src/led.h"
#include "src/loconet-bus.h"
#include "src/outputs.h"
#include "src/settings.h"
#include "src/timer.h"

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
#endif

    led_init();
    timer_init();
    settings_init();
    outputs_init();
    inputs_init();
    buttons_init();
    loconet_init();
    can_init();
    lcc_init();

#ifdef DEBUG
    Serial.println("arcrail initialized");

    #ifdef USE_LCC
    Serial.print(LCC_PRODUCER_CONSUMER_COUNT);
    Serial.println(" producer/consumers");

        #ifndef LCC_USE_BLUE_GOLD
    led_blink(0, 3);
        #endif
    #endif
#endif
}

void loop() {
    // must be called first so other modules can use the timer
    timer_update();

    outputs_update();
    inputs_update();
    buttons_update();
    loconet_update();
    can_update();
    lcc_update();
    led_update();
}

#ifdef BOARD_LCC_DEVELOPMENT_BOARD_REV_A
void lcc_on_message(lcc_mti_t mti, lcc_node_id_alias_t source_nid, uint8_t length, uint8_t *data) {
    #ifdef DEBUG
    Serial.print("LCC raw: mti=");
    Serial.print(mti, HEX);
    Serial.print(", source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", data=");

    for (uint8_t i = 0; i < length; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
    #endif
}

void lcc_on_reserve_id(lcc_node_id_alias_t source_nid) {
    #ifdef DEBUG
    Serial.print("LCC rid: source_nid=");
    Serial.println(source_nid, HEX);
    #endif
}

void lcc_on_alias_map_definition(lcc_node_id_alias_t source_nid, lcc_node_id_t node_id) {
    #ifdef DEBUG
    Serial.print("LCC amd: source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", full_node_id=");

    for (uint8_t i = 0; i < LCC_NODE_ID_LENGTH; i++) {
        Serial.print(node_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
    #endif
}

void lcc_on_initialization_complete(lcc_node_id_alias_t source_nid, lcc_node_id_t node_id) {
    #ifdef DEBUG
    Serial.print("LCC init complete: source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", full_node_id=");

    for (uint8_t i = 0; i < LCC_NODE_ID_LENGTH; i++) {
        Serial.print(node_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
    #endif
}

void lcc_on_verify_node_id(uint8_t length, lcc_node_id_t node_id) {
    #ifdef DEBUG
    Serial.print("LCC verify node id: full_node_id=");

    for (uint8_t i = 0; i < length; i++) {
        Serial.print(node_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
    #endif
}

void lcc_on_verified_node_id(lcc_node_id_t node_id, bool simple_set) {
    #ifdef DEBUG
    Serial.print("LCC verified node id: full_node_id=");

    for (uint8_t i = 0; i < LCC_NODE_ID_LENGTH; i++) {
        Serial.print(node_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", simple_set=");
    Serial.println(simple_set);
    #endif
}

void lcc_on_producer_consumer_event_report(lcc_event_id_t event_id, uint8_t length, uint8_t *payload) {
    #ifdef DEBUG
    Serial.print("LCC pcer: event_id=");

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        Serial.print(event_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", payload=");

    for (uint8_t i = 0; i < length; i++) {
        Serial.print(payload[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
    #endif
}

void lcc_on_producer_identified(lcc_event_id_t event_id, uint8_t event_state) {
    #ifdef DEBUG
    Serial.print("LCC producer identified: event_id=");

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        Serial.print(event_id.data[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", event_state=");
    Serial.println(event_state, HEX);
    #endif
}
#endif
