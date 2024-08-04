#ifdef BOARD_LOCONET_ACCESSORY_DECODER_REV_C
    #include "loconet-accessory-decoder-rev-c.h"

    #include "../cv.h"
    #include "../settings.h"

bool settings_on_programming_helper(uint8_t mode, uint16_t parameter) {
    switch (mode) {
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
#endif
