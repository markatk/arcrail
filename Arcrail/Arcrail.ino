#include "configuration.h"

#include "src/inputs.h"
#include "src/led.h"
#include "src/loconet-bus.h"
#include "src/outputs.h"
#include "src/settings.h"
#include "src/timer.h"

void setup() {
    led_init();
    timer_init();
    settings_init();
    outputs_init();
    inputs_init();
    loconet_init();
}

void loop() {
    outputs_update();
    inputs_update();
    loconet_update();
    led_update();
}
