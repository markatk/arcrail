#include "blue_gold.h"

#ifdef LCC_USE_BLUE_GOLD
    #include "../buttons.h"
    #include "../led.h"

    #define BLUE_MAX_COUNT 6
    #define GOLD_MAX_COUNT 2

uint8_t _blue_state;
uint8_t _gold_state;
#endif

void blue_gold_init() {
#ifdef LCC_USE_BLUE_GOLD

#endif
}

void blue_gold_update() {
#ifdef LCC_USE_BLUE_GOLD
    if (button_is_just_pressed(LCC_BLUE_BUTTON)) {
        _blue_state += 1;

        if (_blue_state > BLUE_MAX_COUNT) {
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

    if (button_is_just_pressed(LCC_GOLD_BUTTON)) {
        _gold_state += 1;

        if (_gold_state > GOLD_MAX_COUNT) {
            _gold_state = 0;

            // turn off led since the state was rolled over
    #ifdef LCC_GOLD_LED
            led_set(LCC_GOLD_LED, LOW);
    #endif
        }
    #ifdef LCC_GOLD_LED
        else {
            led_blink(LCC_GOLD_LED, _gold_state);
        }
    #endif
    }
#endif
}

void blue_gold_reset() {
#ifdef LCC_USE_BLUE_GOLD
    _blue_state = 0;
    _gold_state = 0;

    #ifdef LCC_BLUE_LED
    led_set(LCC_BLUE_LED, LOW);
    #endif

    #ifdef LCC_GOLD_LED
    led_set(LCC_GOLD_LED, LOW);
    #endif
#endif
}

#ifdef LCC_USE_BLUE_GOLD

#endif
