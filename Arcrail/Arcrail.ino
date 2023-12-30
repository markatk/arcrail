#include "configuration.h"

#include "inputs.h"
#include "led.h"
#include "outputs.h"
#include "settings.h"
#include "timer.h"

#ifdef USE_LOCONET
    #include "loconet-bus.h"
#endif

void setup() {
    led_init();
    timer_init();
    settings_init();
    outputs_init();
    inputs_init();

#ifdef USE_LOCONET
    loconet_init();
#endif
}

void loop() {
    outputs_update();
    inputs_update();

#ifdef USE_LOCONET
    loconet_update();
#endif

    led_update();
}
