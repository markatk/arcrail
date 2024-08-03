#pragma once

#include "configuration.h"

#ifdef USE_OUTPUTS
    #define CV_OUTPUT_TURN_ON_BASE 21
    #define CV_OUTPUT_TURN_OFF_BASE 41
    #define CV_SWITCHING_MODE_BASE 61
    #define CV_SWITCHING_2ND_PARAMETER_BASE 81
    #define CV_OUTPUT_DELAY_BASE 101
#endif

#ifdef USE_INPUTS
    #define CV_INPUT_ADDRESS_BASE 201
    #define CV_INPUT_ADDRESS_DELAY 221
#endif
