#include "inputs.h"

#include "../configuration.h"
#include "settings.h"
#include "timer.h"

#ifdef USE_LOCONET
    #include "loconet-bus.h"
#endif

#ifdef USE_INPUTS
uint8_t _states[INPUT_COUNT];
uint16_t _state_counters[INPUT_COUNT];

static void send_state(uint8_t input, uint8_t state);
#endif

void inputs_init() {
#ifdef USE_INPUTS
    for (auto i = 0; i < INPUT_COUNT; i++) {
        pinMode(INPUTS[i], INPUT);

        _states[i] = 0xFF;
        _state_counters[i] = 0;
    }
#endif
}

void inputs_update() {
#ifdef USE_INPUTS
    // wait till timer was triggered
    if (timer_was_triggered() == false) {
        return;
    }

    for (auto i = 0; i < INPUT_COUNT; i++) {
        auto value = digitalRead(INPUTS[i]);
        if (_states[i] != 0xFF && value == _states[i]) {
            // decrease counter if it was counting because state did (maybe) not change
            if (_state_counters[i] > 0) {
                _state_counters[i] -= 1;
            }

            continue;
        }

        // increase and compare state change timer
        _state_counters[i] += 1;

        uint16_t input_delay;
        if (settings_get_input_delay(i, &input_delay) == false || _state_counters[i] < input_delay) {
            continue;
        }

        // state change was verified
        _states[i] = value;
        _state_counters[i] = 0;

        send_state(i, value);
    }
#endif
}

void inputs_reset() {
#ifdef USE_INPUTS
    for (auto i = 0; i < INPUT_COUNT; i++) {
        // read current value and use is as new state
        auto value = digitalRead(INPUTS[i]);
        _states[i] = value;
        _state_counters[i] = 0;

        send_state(i, value);
    }
#endif
}

#ifdef USE_INPUTS
void send_state(uint8_t input, uint8_t state) {
    uint16_t address;
    if (settings_get_input_address(input, &address) == false) {
        return;
    }

    #ifdef USE_LOCONET
        #ifdef INVERT_INPUTS
    loconet_report_sensor(address, !state);
        #else
    loconet_report_sensor(address, state);
        #endif
    #endif
}
#endif
