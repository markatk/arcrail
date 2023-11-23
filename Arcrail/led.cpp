#include "led.h"

void led_init() {
#ifdef STATUS_LED
    pinMode(PIN_STATUS_LED, OUTPUT);

    status_led_set(LOW);
#endif
}

void status_led_set(uint8_t value) {
#ifdef STATUS_LED_INVERT_OUTPUT
    digitalWrite(PIN_STATUS_LED, !value);
#else
    digitalWrite(PIN_STATUS_LED, value);
#endif
}
