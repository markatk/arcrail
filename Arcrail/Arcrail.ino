#include "configuration.h"

#include "led.h"
#include "outputs.h"
#include "settings.h"
#include "timer.h"

void setup() {
    led_init();
    timer_init();
    settings_init();

#ifdef USE_OUTPUTS
    outputs_init();
#endif

    status_led_set(true);
}

void loop() {
#ifdef USE_OUTPUTS
    outputs_update();
#endif
}
