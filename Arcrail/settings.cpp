#include "settings.h"

#include "configuration.h"
#include <EEPROM.h>

#define CV_OUTPUT_TURN_ON_BASE 21
#define CV_OUTPUT_TURN_OFF_BASE 41
#define CV_SWITCHING_MODE_BASE 61
#define CV_SWITCHING_2ND_PARAMETER_BASE 81
#define CV_OUTPUT_DELAY_BASE 101
#define CV_INPUT_ADDRESS_BASE 201
#define CV_INPUT_ADDRESS_DELAY 221

bool _is_valid_cv(uint16_t cv);
bool _handle_programming_helper(uint16_t value);

void settings_init() {
    // check if initial configuration has to be set
    if (EEPROM.read(0) != 0xFF || EEPROM.read(1) != 0xFF) {
        return;
    }

    settings_set_value(0, SETTINGS_DEFAULT_MODULE_VALUE);
    settings_set_value(1, SETTINGS_DEFAULT_MODE_VALUE);
    settings_set_value(20, FIRMWARE_VERSION);
}

bool settings_set_value(uint16_t address, uint16_t value) {
    if (_is_valid_cv(address) == false) {
        return false;
    }

    // values are stored in little endian
    EEPROM.update(address * 2, value & 0xFF);
    EEPROM.update(address * 2 + 1, value >> 8);

    if (address == 1) {
        return _handle_programming_helper(value);
    }

    return true;
}

bool settings_get_value(uint16_t address, uint16_t *value) {
    if (_is_valid_cv(address) == false) {
        return false;
    }

    // values are stored in little endian
    *value = EEPROM.read(address * 2);
    *value |= EEPROM.read(address * 2 + 1) << 8;

    return true;
}

bool settings_get_output_turn_on_address(uint8_t output, uint16_t *address, uint8_t *direction) {
#ifdef USE_OUTPUTS
    if (output >= OUTPUT_COUNT) {
        return false;
    }

    uint16_t value;
    if (settings_get_value(CV_OUTPUT_TURN_ON_BASE + output, &value) == false) {
        return false;
    }

    // turn-on address stores the address and red/green direction which needs to be separated
    if (address != NULL) {
        *address = value / 10;
    }

    if (direction != NULL) {
        *direction = value % 10;
    }

    return true;
#else
    return false;
#endif
}

bool settings_get_output_turn_off_address(uint8_t output, uint16_t *address, uint8_t *direction) {
#ifdef USE_OUTPUTS
    if (output >= OUTPUT_COUNT) {
        return false;
    }

    uint16_t value;
    if (settings_get_value(CV_OUTPUT_TURN_OFF_BASE + output, &value) == false) {
        return false;
    }

    // turn-off address stores the address and red/green direction which needs to be separated
    if (address != NULL) {
        *address = value / 10;
    }

    if (direction != NULL) {
        *direction = value % 10;
    }

    return true;
#else
    return false;
#endif
}

bool settings_get_output_switching_mode(uint8_t output, uint8_t *mode, uint16_t *parameter) {
#ifdef USE_OUTPUTS
    if (output >= OUTPUT_COUNT) {
        return false;
    }

    uint16_t value;
    if (settings_get_value(CV_SWITCHING_MODE_BASE + output, &value) == false) {
        return false;
    }

    // cv stores mode and time which needs to be separated
    if (mode != NULL) {
        *mode = value % 10;
    }

    if (parameter != NULL) {
        *parameter = value / 10;
    }

    return true;
#else
    return false;
#endif
}

bool settings_get_output_switching_2nd_parameter(uint8_t output, bool *enabled, uint16_t *parameter) {
#ifdef USE_OUTPUTS
    if (output >= OUTPUT_COUNT) {
        return false;
    }

    uint16_t value;
    if (settings_get_value(CV_SWITCHING_2ND_PARAMETER_BASE + output, &value) == false) {
        return false;
    }

    // cv stores mode and time which needs to be separated
    if (enabled != NULL) {
        *enabled = (value % 10) != 0;
    }

    if (parameter != NULL) {
        *parameter = value / 10;
    }

    return true;
#else
    return false;
#endif
}

bool settings_get_output_delay(uint8_t output, uint16_t *delay) {
#ifdef USE_OUTPUTS
    if (output >= OUTPUT_COUNT) {
        return false;
    }

    return settings_get_value(CV_OUTPUT_DELAY_BASE + output, delay);
#else
    return false;
#endif
}

bool settings_has_output_delay(uint8_t output) {
#ifdef USE_OUTPUTS
    uint16_t value = 0;
    if (settings_get_output_delay(output, &value) == false) {
        return false;
    }

    return value > 0;
#else
    return false;
#endif
}

bool settings_get_input_address(uint8_t input, uint16_t *address) {
#ifdef USE_INPUTS
    if (input >= INPUT_COUNT) {
        return false;
    }

    return settings_get_value(CV_INPUT_ADDRESS_BASE + input, address);
#else
    return false;
#endif
}

bool settings_get_input_delay(uint8_t input, uint16_t *delay) {
#ifdef USE_INPUTS
    if (input >= INPUT_COUNT) {
        return false;
    }

    return settings_get_value(CV_INPUT_ADDRESS_DELAY + input, delay);
#else
    return false;
#endif
}

bool _is_valid_cv(uint16_t cv) {
    // module address, programming helper and other various addresses
    if (cv <= 1 || cv == 20) {
        return true;
    }

#ifdef USE_OUTPUTS
    // output configurations
    if (cv >= CV_OUTPUT_TURN_ON_BASE && cv < CV_OUTPUT_TURN_ON_BASE + OUTPUT_COUNT) {
        return true;
    }

    if (cv >= CV_OUTPUT_TURN_OFF_BASE && cv < CV_OUTPUT_TURN_OFF_BASE + OUTPUT_COUNT) {
        return true;
    }

    if (cv >= CV_SWITCHING_MODE_BASE && cv < CV_SWITCHING_MODE_BASE + OUTPUT_COUNT) {
        return true;
    }

    if (cv >= CV_SWITCHING_2ND_PARAMETER_BASE && cv < CV_SWITCHING_2ND_PARAMETER_BASE + OUTPUT_COUNT) {
        return true;
    }

    if (cv >= CV_OUTPUT_DELAY_BASE && cv < CV_OUTPUT_DELAY_BASE + OUTPUT_COUNT) {
        return true;
    }
#endif

#ifdef USE_INPUTS
    // input configurations
    if (cv >= CV_INPUT_ADDRESS_BASE && cv < CV_INPUT_ADDRESS_BASE + INPUT_COUNT) {
        return true;
    }

    if (cv >= CV_INPUT_ADDRESS_DELAY && cv < CV_INPUT_ADDRESS_DELAY + INPUT_COUNT) {
        return true;
    }
#endif

    return false;
}

bool _handle_programming_helper(uint16_t value) {
    uint8_t mode = value % 10;
    uint16_t parameter = value / 10;

    // TODO: Move programming helper into custom boards?
    switch (mode) {
        // do nothing
        case 0:
            return true;

        // program outputs to listen on consecutive addresses as permanent outputs
        case 1:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, (parameter + i) * 10);
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, (parameter + i) * 10 + 1);
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 0);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 0);
            }

            return true;

        // program output pairs with switching time
        case 2:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i += 2) {
                // set turn-on address to parameter value on red and green direction
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, (parameter + i / 2) * 10 + (i % 2));
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i + 1, (parameter + i / 2) * 10 + ((i + 1) % 2));

                // set turn-off address to opposite parameter value on red and green direction
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, (parameter + i / 2) * 10 + ((i + 1) % 2));
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i + 1, (parameter + i / 2) * 10 + (i % 2));

                // set switching times to 1 second
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 101);
                settings_set_value(CV_SWITCHING_MODE_BASE + i + 1, 101);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 0);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i + 1, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i + 1, 0);
            }

            return true;

        // program output pairs with permanent output
        case 3:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i += 2) {
                // set turn-on address to parameter value on red and green direction
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, (parameter + i / 2) * 10 + (i % 2));
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i + 1, (parameter + i / 2) * 10 + ((i + 1) % 2));

                // set turn-off address to opposite parameter value on red and green direction
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, (parameter + i / 2) * 10 + ((i + 1) % 2));
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i + 1, (parameter + i / 2) * 10 + (i % 2));

                // set switching times to 1 second
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 0);
                settings_set_value(CV_SWITCHING_MODE_BASE + i + 1, 0);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 0);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i + 1, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i + 1, 0);
            }

            return true;

        // blink every output 500ms on given address
        case 4:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, (parameter + i) * 10);
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, (parameter + i) * 10 + 1);
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 102);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 0);
            }

            return true;

        // light chaser on all 16 outputs on given address
        case 5:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, parameter * 10);
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, parameter * 10 + 1);
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 162);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 2 * i);
            }

            return true;

        // alternating blinking on all 16 outputs on given address
        case 6:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, parameter * 10);
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, parameter * 10 + 1);
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 102);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 10 * (i % 2));
            }

            return true;

        // every output listens to the same address and does long duration random turn on/off
        case 7:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, parameter * 10);
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, parameter * 10 + 1);
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 5003);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 1001);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, random(100));
            }

            return true;

        default:
            return false;
    }
}
