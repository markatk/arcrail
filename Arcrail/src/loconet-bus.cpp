#include "loconet-bus.h"

#include "../configuration.h"
#include "led.h"
#include "settings.h"

#ifdef USE_OUTPUTS
    #include "outputs.h"
#endif

#ifdef USE_INPUTS
    #include "inputs.h"
#endif

#ifdef USE_LOCONET
    #include <LocoNet.h>

bool _programming = false;

LocoNetCVClass _lnCV;

uint16_t _get_module_address();
#endif

void loconet_init() {
#ifdef USE_LOCONET
    // initialize loconet
    LocoNet.init(PIN_LOCONET_TX);
#endif
}

void loconet_update() {
#ifdef USE_LOCONET
    // check if any loconet packet is available
    auto packet = LocoNet.receive();
    if (packet == NULL) {
        return;
    }

    auto processed = LocoNet.processSwitchSensorMessage(packet);

    // processLNCVMessage must be called to trigger LNCV notify callbacks
    if (processed == 0) {
        _lnCV.processLNCVMessage(packet);
    }
#endif
}

#ifdef USE_LOCONET
void loconet_report_sensor(uint16_t address, uint8_t state) {
    LocoNet.reportSensor(address, state);
}

void notifyPower(uint8_t state) {
    #ifdef USE_INPUTS
    if (state) {
        inputs_reset();
    }
    #endif
}

void notifySwitchRequest(uint16_t address, uint8_t output, uint8_t direction) {
    #ifdef USE_OUTPUTS
    // only handle requests which turn on switches
    if (output == 0) {
        return;
    }

    outputs_parse(address, direction);
    #endif
}

void notifySwitchState(uint16_t address, uint8_t output, uint8_t direction) {
    #ifdef USE_OUTPUTS
    // only handle requests which turn on switches
    if (output == 0) {
        return;
    }

    outputs_parse(address, direction);
    #endif
}

void notifySwitchOutputsReport(uint16_t address, uint8_t closedOutput, uint8_t throwOutput) {
}

void notifySwitchReport(uint16_t address, uint8_t state, uint8_t sensor) {
}

int8_t notifyLNCVprogrammingStart(uint16_t &typeNumber, uint16_t &moduleAddress) {
    if (typeNumber != LOCONET_TYPE_NUMBER) {
        return -1;
    }

    // listen for own module and broadcast address
    auto address = _get_module_address();

    if (moduleAddress != address && moduleAddress != 0xFFFF) {
        return -1;
    }

    _programming = true;

    #ifdef STATUS_LED
    status_led_blink();
    #endif

    return LNCV_LACK_OK;
}

void notifyLNCVprogrammingStop(uint16_t typeNumber, uint16_t moduleAddress) {
    // listen for broadcast stop
    if (typeNumber == 0xFFFF) {
        _programming = false;

        return;
    }

    // listen for own type number and module address
    auto address = _get_module_address();

    if (typeNumber != LOCONET_TYPE_NUMBER || moduleAddress != address) {
        return;
    }

    _programming = false;

    #ifdef STATUS_LED
    status_led_set(false);
    #endif
}

int8_t notifyLNCVread(uint16_t typeNumber, uint16_t cv, uint16_t &value) {
    if (_programming == false || typeNumber != LOCONET_TYPE_NUMBER) {
        return -1;
    }

    // return positive value if cv was invalid
    if (settings_get_value(cv, &value) == false) {
        return 1;
    }

    return LNCV_LACK_OK;
}

int8_t notifyLNCVwrite(uint16_t typeNumber, uint16_t cv, uint16_t value) {
    if (_programming == false || typeNumber != LOCONET_TYPE_NUMBER) {
        return -1;
    }

    // return positive value if cv was invalid
    if (settings_set_value(cv, value) == false) {
        return 1;
    }

    return LNCV_LACK_OK;
}

uint16_t _get_module_address() {
    uint16_t value = 0;

    settings_get_value(0, &value);

    return value;
}
#endif
