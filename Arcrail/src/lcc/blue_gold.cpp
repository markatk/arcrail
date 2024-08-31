#include "blue_gold.h"

#ifdef LCC_USE_BLUE_GOLD
    #include "../buttons.h"
    #include "../led.h"
    #include "lcc.h"
    #include "mti.h"
    #include "producer_consumer.h"
    #include "transport.h"

uint8_t _blue_state;
uint8_t _gold_state;
bool _learn_states[LCC_PRODUCER_CONSUMER_COUNT];

void _set_blue_state(uint8_t state);
void _set_gold_state(uint8_t state);
void _on_learn();
void _on_teach();
#endif

void blue_gold_init() {
}

void blue_gold_update() {
#ifdef LCC_USE_BLUE_GOLD
    if (button_is_just_pressed(LCC_BLUE_BUTTON)) {
        _set_blue_state(_blue_state + 1);

        if (_gold_state == LCC_BLUE_GOLD_MODE_OFF) {
            _set_gold_state(LCC_BLUE_GOLD_MODE_LEARN);
        }
    }

    if (button_is_just_pressed(LCC_GOLD_BUTTON)) {
        switch (_gold_state) {
            case LCC_BLUE_GOLD_MODE_OFF:
                _set_gold_state(LCC_BLUE_GOLD_MODE_TEACH);
                _set_blue_state(1);
                break;

            case LCC_BLUE_GOLD_MODE_LEARN:
                _on_learn();
                break;

            case LCC_BLUE_GOLD_MODE_TEACH:
                _on_teach();
                _set_gold_state(LCC_BLUE_GOLD_MODE_OFF);
                _set_blue_state(0);
                break;
        }
    }
#endif
}

void blue_gold_reset() {
#ifdef LCC_USE_BLUE_GOLD
    _blue_state = 0;
    _gold_state = 0;

    for (uint8_t i = 0; i < LCC_PRODUCER_CONSUMER_COUNT; i++) {
        _learn_states[i] = false;
    }

    #ifdef LCC_BLUE_LED
    led_set(LCC_BLUE_LED, LOW);
    #endif

    #ifdef LCC_GOLD_LED
    led_set(LCC_GOLD_LED, LOW);
    #endif
#endif
}

#ifdef LCC_USE_BLUE_GOLD
void blue_gold_learn(lcc_event_id_t event_id) {
    // also flag current selection as learning
    uint8_t index = blue_gold_get_selection();
    if (index != 0xFF) {
        _learn_states[index] = true;
    }

    // update event id in every producer/consumer flagged to learn
    for (uint8_t i = 0; i < LCC_PRODUCER_CONSUMER_COUNT; i++) {
        if (_learn_states[i] == false) {
            return;
        }

        producer_consumer_set_event_id(i, event_id);

        _learn_states[i] = false;
    }

    _set_gold_state(LCC_BLUE_GOLD_MODE_OFF);
    _set_blue_state(0);
}

uint8_t blue_gold_get_mode() {
    return _gold_state;
}

uint8_t blue_gold_get_selection() {
    if (_blue_state == 0 || _blue_state > LCC_PRODUCER_CONSUMER_COUNT) {
        return 0xFF;
    }

    return _blue_state - 1;
}

void _set_blue_state(uint8_t state) {
    _blue_state = state;

    if (_blue_state > LCC_PRODUCER_CONSUMER_COUNT) {
        _blue_state = 0;

        // turn off led since the state was rolled over
    #ifdef LCC_BLUE_LED
        led_set(LCC_BLUE_LED, LOW);
    #endif
    }
    #ifdef LCC_BLUE_LED
    else {
        led_blink(LCC_BLUE_LED, _blue_state);
    }
    #endif
}

void _set_gold_state(uint8_t state) {
    _gold_state = state;

    #ifdef LCC_GOLD_LED
    if (_gold_state > 0) {
        led_blink(LCC_GOLD_LED, _gold_state);
    } else {
        led_set(LCC_GOLD_LED, LOW);
    }
    #endif
}

void _on_learn() {
    uint8_t index = blue_gold_get_selection();
    if (index >= LCC_PRODUCER_CONSUMER_COUNT) {
        return;
    }

    _learn_states[index] = true;

    Serial.print("Learning producer/consumer ");
    Serial.println(index);

    // reset selection to first
    _set_blue_state(1);
}

void _on_teach() {
    lcc_event_id_t event_id;
    if (producer_consumer_get_event_id(blue_gold_get_selection(), &event_id) == false) {
        return;
    }

    // send learn event
    transport_send(MTI_LEARN_EVENT, LCC_EVENT_ID_LENGTH, event_id.data);
}
#endif
