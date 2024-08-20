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
#ifdef BOARD_LCC_DEVELOPMENT_BOARD_REV_A
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

#ifdef BOARD_LCC_DEVELOPMENT_BOARD_REV_A
    Serial.println("arcrail initialized");

    #ifndef LCC_USE_BLUE_GOLD
    led_blink(0, 3);
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
void lcc_on_message(uint16_t mti, uint16_t source_nid, uint8_t length, uint8_t *data) {
    led_flash(2);

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
}

void lcc_on_reserve_id(uint16_t source_nid) {
    Serial.print("LCC rid: source_nid=");
    Serial.println(source_nid, HEX);
}

void lcc_on_alias_map_definition(uint16_t source_nid, uint8_t *full_node_id) {
    Serial.print("LCC amd: source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", full_node_id=");

    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        Serial.print(full_node_id[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
}

void lcc_on_initialization_complete(uint16_t source_nid, uint8_t *full_node_id) {
    Serial.print("LCC init complete: source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", full_node_id=");

    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        Serial.print(full_node_id[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
}

void lcc_on_verify_node_id(uint8_t length, uint8_t *full_node_id) {
    Serial.print("LCC verify node id: full_node_id=");

    for (uint8_t i = 0; i < length; i++) {
        Serial.print(full_node_id[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
}

void lcc_on_verified_node_id(uint8_t *full_node_id, bool simple_set) {
    Serial.print("LCC verified node id: full_node_id=");

    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        Serial.print(full_node_id[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", simple_set=");
    Serial.println(simple_set);
}

void lcc_on_producer_consumer_event_report(uint8_t *full_node_id, uint16_t event, uint8_t length, uint8_t *payload) {
    Serial.print("LCC pcer: full_node_id=");

    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        Serial.print(full_node_id[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", event=");
    Serial.print(event, HEX);
    Serial.print(", payload=");

    for (uint8_t i = 0; i < length; i++) {
        Serial.print(payload[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
}

void lcc_on_producer_identified(uint8_t *event_id, uint8_t event_state) {
    Serial.print("LCC producer identified: event_id=");

    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        Serial.print(event_id[i], HEX);
        Serial.print(" ");
    }

    Serial.print(", event_state=");
    Serial.println(event_state, HEX);
}
#endif
