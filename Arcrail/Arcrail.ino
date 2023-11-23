#include "configuration.h"

#include "led.h"
#include "settings.h"
#include "timer.h"

void setup() {
    led_init();
    timer_init();
    settings_init();

    status_led_set(true);
}

void loop() {
}
