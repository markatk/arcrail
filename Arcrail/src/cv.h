#pragma once

#include "../configuration.h"

#define CV_FIRMWARE 20

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

#ifdef USE_LCC
    #define CV_LCC_NODE_ID_0_1 2
    #define CV_LCC_NODE_ID_2_3 3
    #define CV_LCC_NODE_ID_4_5 4
#endif
