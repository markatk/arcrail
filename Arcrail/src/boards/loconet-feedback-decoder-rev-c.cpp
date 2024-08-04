#include "../../configuration.h"

#ifdef BOARD_LOCONET_FEEDBACK_DECODER_REV_C
    #include "loconet-feedback-decoder-rev-c.h"

    #include "../cv.h"
    #include "../settings.h"

bool settings_on_programming_helper(uint8_t mode, uint16_t parameter) {
    switch (mode) {
        // program inputs with consecutive addresses
        case 1:
            for (uint8_t i = 0; i < INPUT_COUNT; i++) {
                settings_set_value(CV_INPUT_ADDRESS_BASE + i, parameter + i);
                settings_set_value(CV_INPUT_ADDRESS_DELAY + i, 0);
            }

            return true;

        default:
            return false;
    }
}
#endif
