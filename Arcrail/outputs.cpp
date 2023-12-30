#include "outputs.h"

#include "configuration.h"
#include "settings.h"
#include "timer.h"

#ifdef USE_OUTPUTS
uint8_t _switching_mode[OUTPUT_COUNT];
uint8_t _switching_time[OUTPUT_COUNT];
uint8_t _switching_parameter[OUTPUT_COUNT];
uint16_t _switching_delay[OUTPUT_COUNT];

void _configure_switching_mode(uint8_t output);
uint8_t read_output(uint8_t output);
void write_output(uint8_t output, uint8_t value);
void toggle_output(uint8_t output);
#endif

void outputs_init() {
#ifdef USE_OUTPUTS
    for (auto i = 0; i < OUTPUT_COUNT; i++) {
        pinMode(OUTPUTS[i], OUTPUT);

        write_output(i, LOW);

        _switching_time[i] = 0;
        _switching_mode[i] = SWITCHING_MODE_OFF;
        _switching_parameter[i] = 0;
        _switching_delay[i] = 0;
    }
#endif
}

void outputs_update() {
#ifdef USE_OUTPUTS
    // wait till timer was triggered
    if (timer_was_triggered() == false) {
        return;
    }

    // decrease counter of each output if switching time is configured
    for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
        if (_switching_mode[i] == SWITCHING_MODE_OFF) {
            continue;
        }

        // if a delay is set wait till the delay is over
        if (_switching_delay[i] > 1) {
            _switching_delay[i] -= 1;

            continue;
        }

        // trigger delayed action
        if (_switching_delay[i] == 1) {
            _switching_delay[i] = 0;

            switch (_switching_mode[i]) {
                case SWITCHING_MODE_TIME:
                    write_output(i, LOW);

                    break;

                case SWITCHING_MODE_BLINK:
                case SWITCHING_MODE_RANDOM:
                    toggle_output(i);

                    break;

                default:
                    break;
            }

            continue;
        }

        // wait till the next action for this output is required
        if (_switching_time[i] > 1) {
            _switching_time[i] -= 1;

            continue;
        }

        // trigger the output according to the configured mode
        switch (_switching_mode[i]) {
            case SWITCHING_MODE_TIME:
                write_output(i, LOW);

                _switching_mode[i] = SWITCHING_MODE_OFF;

                break;

            case SWITCHING_MODE_BLINK:
                toggle_output(i);

                _switching_time[i] = _switching_parameter[i];

                break;

            case SWITCHING_MODE_RANDOM:
                toggle_output(i);

                _switching_time[i] = random(_switching_parameter[i]);

                break;

            default:
                break;
        }
    }
#endif
}

void outputs_parse(uint16_t address, bool direciton) {
#ifdef USE_OUTPUTS
    // check each output if given address is assigned
    // loop is needed since more than 1 output can listen to the same address
    uint16_t output_address;
    uint8_t output_direction;

    for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
        if (settings_get_output_turn_on_address(i, &output_address, &output_direction)) {
            // TODO: Support option to ignore direction
            if (output_address == address && output_direction == direciton) {
                // only enable the output if no delay is configured
                if (settings_has_output_delay(i) == false) {
                    write_output(i, HIGH);
                }

                _configure_switching_mode(i);
            }
        }

        if (settings_get_output_turn_off_address(i, &output_address, &output_direction)) {
            // TODO: Support option to ignore direction
            if (output_address == address && output_direction == direciton) {
                write_output(i, LOW);

                // reset switching time so output is not triggered
                _switching_time[i] = 0;
                _switching_mode[i] = SWITCHING_MODE_OFF;
            }
        }
    }
#endif
}

#ifdef USE_OUTPUTS
void _configure_switching_mode(uint8_t output) {
    // check if any switching mode is configured
    uint8_t switching_mode;
    uint16_t switching_parameter;
    uint16_t output_delay;

    if (settings_get_output_switching_mode(output, &switching_mode, &switching_parameter) == false || settings_get_output_delay(output, &output_delay) == false) {
        return;
    }

    // set switching mode and time according to mode
    _switching_mode[output] = switching_mode;
    _switching_parameter[output] = switching_parameter;
    _switching_delay[output] = output_delay;

    switch (switching_mode) {
        case SWITCHING_MODE_TIME:
        case SWITCHING_MODE_BLINK:
            _switching_time[output] = switching_parameter;

            break;

        case SWITCHING_MODE_RANDOM:
            _switching_time[output] = random(switching_parameter);

            break;

        default:
            break;
    }
}

uint8_t read_output(uint8_t output) {
    return digitalRead(OUTPUTS[output]);
}

void write_output(uint8_t output, uint8_t value) {
    #ifdef INVERT_OUTPUTS
    digitalWrite(OUTPUTS[output], !value);
    #else
    digitalWrite(OUTPUTS[output], value);
    #endif
}

void toggle_output(uint8_t output) {
    write_output(output, read_output(output));
}
#endif
