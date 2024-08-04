#include "configuration.h"

#include "src/inputs.h"
#include "src/led.h"
#include "src/outputs.h"
#include "src/settings.h"
#include "src/timer.h"

#ifdef USE_LOCONET
    #include "src/loconet-bus.h"
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
