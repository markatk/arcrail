#ifdef BOARD_LOCONET_TURNOUT_DECODER_REV_A
    #include "loconet-turnout-decoder-rev-a.h"

    #include "../cv.h"
    #include "../settings.h"

bool settings_on_programming_helper(uint8_t mode, uint16_t parameter) {
    switch (mode) {
        // program output pairs with switching time
        case 1:
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
        case 2:
            for (uint8_t i = 0; i < OUTPUT_COUNT; i += 2) {
                // set turn-on address to parameter value on red and green direction
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i, (parameter + i / 2) * 10 + (i % 2));
                settings_set_value(CV_OUTPUT_TURN_ON_BASE + i + 1, (parameter + i / 2) * 10 + ((i + 1) % 2));

                // set turn-off address to opposite parameter value on red and green direction
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i, (parameter + i / 2) * 10 + ((i + 1) % 2));
                settings_set_value(CV_OUTPUT_TURN_OFF_BASE + i + 1, (parameter + i / 2) * 10 + (i % 2));

                // set switching times to permanent
                settings_set_value(CV_SWITCHING_MODE_BASE + i, 0);
                settings_set_value(CV_SWITCHING_MODE_BASE + i + 1, 0);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i, 0);
                settings_set_value(CV_SWITCHING_2ND_PARAMETER_BASE + i + 1, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i, 0);
                settings_set_value(CV_OUTPUT_DELAY_BASE + i + 1, 0);
            }

            return true;

        default:
            return false;
    }
}
#endif
