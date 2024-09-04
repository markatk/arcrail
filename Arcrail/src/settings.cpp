#include "settings.h"

#include "../configuration.h"
#include "cv.h"
#include <EEPROM.h>

bool _is_valid_cv(uint16_t cv);
bool _handle_programming_helper(uint16_t value);

void settings_init() {
    // check if initial configuration has to be set
    if (EEPROM.read(CV_FIRMWARE * 2) != 0xFF && EEPROM.read(CV_FIRMWARE * 2 + 1) != 0xFF) {
        return;
    }

    // firmware version cannot be written via settings function
    EEPROM.update(CV_FIRMWARE * 2, FIRMWARE_VERSION & 0xFF);
    EEPROM.update(CV_FIRMWARE * 2 + 1, FIRMWARE_VERSION >> 8);

#ifdef USE_INPUTS
    for (uint8_t i = 0; i < INPUT_COUNT; i++) {
        settings_set_value(CV_INPUT_ADDRESS_DELAY + i, 0);
    }
#endif

#ifdef USE_LOCONET
    settings_set_value(0, SETTINGS_DEFAULT_MODULE_VALUE);
    settings_set_value(1, SETTINGS_DEFAULT_MODE_VALUE);
#else
    for (uint8_t i = 0; i < OUTPUT_COUNT; i++) {
        settings_set_value(CV_SWITCHING_MODE_BASE + i, 0);
        settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 0);
        settings_set_value(CV_OUTPUT_DELAY_BASE + i, 0);
    }
#endif
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
    if (address != CV_FIRMWARE && _is_valid_cv(address) == false) {
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

__attribute__((weak)) bool settings_on_programming_helper(uint8_t mode, uint16_t parameter) {
    return false;
}

bool _is_valid_cv(uint16_t cv) {
    // module address, programming helper and other various addresses
#ifdef USE_LOCONET
    if (cv <= 1) {
        return true;
    }
#endif

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

    if (mode == 0) {
        // do nothing
        return true;
    }

    return settings_on_programming_helper(mode, parameter);
}
