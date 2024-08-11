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
    Serial.begin(9600);

    led_init();
    timer_init();
    settings_init();
    outputs_init();
    inputs_init();
    buttons_init();
    loconet_init();
    can_init();
    lcc_init();

    led_blink(0);
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

void lcc_on_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
    led_flash(2);

    Serial.print("LCC: content_field=");
    Serial.print(content_field, HEX);
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
    Serial.print("LCC reserve id: source_nid=");
    Serial.println(source_nid, HEX);
}

void lcc_on_alias_map_definition(uint16_t source_nid, uint8_t *full_node_id) {
    Serial.print("LCC amd: source_nid=");
    Serial.print(source_nid, HEX);
    Serial.print(", full_node_id=");

    for (uint8_t i = 0; i < NODE_ID_LENGTH; i++) {
        Serial.print(full_node_id[i]);
        Serial.print(" ");
    }

    Serial.println();
}
