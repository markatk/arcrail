#include "configuration.h"

#include "led.h"

void setup() {
    led_init();

    status_led_set(true);
}

void loop() {
}
