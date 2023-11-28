#include "configuration.h"

#include "led.h"
#include "loconet-bus.h"
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

#ifdef USE_LOCONET
    loconet_init();
#endif
}

void loop() {
#ifdef USE_OUTPUTS
    outputs_update();
#endif

#ifdef USE_LOCONET
    loconet_update();
#endif

    led_update();
}
