#pragma once

#include <Arduino.h>

//===========================================================================
//======================== Boards ===========================================
//===========================================================================

// uncomment the desired board in the following section or define a custom
// board in the following section
#define BOARD_LOCONET_ACCESSORY_DECODER_REV_C
// #define BOARD_LOCONET_ACCESSORY_DECODER_REV_D
// #define BOARD_LOCONET_FEEDBACK_DECODER_REV_A

#ifdef BOARD_LOCONET_ACCESSORY_DECODER_REV_C
    #include "boards/loconet-accessory-decoder-rev-c.h"
#endif

#ifdef BOARD_LOCONET_ACCESSORY_DECODER_REV_D
    #include "boards/loconet-accessory-decoder-rev-d.h"
#endif

#ifdef BOARD_LOCONET_FEEDBACK_DECODER_REV_A
    #include "boards/loconet-feedback-decoder-rev-a.h"
#endif

//===========================================================================
//======================== Firmware =========================================
//===========================================================================

#ifndef FIRMWARE_VERSION
    #define FIRMWARE_VERSION 1
#endif

//===========================================================================
//======================== Loconet ==========================================
//===========================================================================

#ifdef LOCONET
    #ifndef PIN_LOCONET_RX
        #error Loconet rx pin is not defined
    #endif

    #ifndef PIN_LOCONET_TX
        #error Loconet tx pin is not defined
    #endif

    #ifndef LOCONET_TYPE_NUMBER
        #define LOCONET_TYPE_NUMBER 5201 // type number is expected without last digit
    #endif
#endif

//===========================================================================
//======================== LCC ==============================================
//===========================================================================

//===========================================================================
//======================== Outputs ==========================================
//===========================================================================

#ifdef USE_OUTPUTS
    #ifndef OUTPUT_COUNT
        #error Output count is not defined
    #endif
#endif

//===========================================================================
//======================== Inputs ===========================================
//===========================================================================

#ifdef USE_INPUTS
    #ifndef INPUT_COUNT
        #error Input count is not defined
    #endif
#endif

//===========================================================================
//======================== LEDs =============================================
//===========================================================================

#ifdef STATUS_LED
    #ifndef PIN_STATUS_LED
        #error Status led pin is not defined
    #endif

// #define STATUS_LED_INVERT_OUTPUT
#endif

//===========================================================================
//======================== Buttons ==========================================
//===========================================================================

#ifndef BUTTON_PRESS_COUNT
    #define BUTTON_PRESS_COUNT 200
#endif

#ifndef BUTTON_RELEASE_COUNT
    #define BUTTON_RELEASE_COUNT 20
#endif
