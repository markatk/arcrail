#pragma once

#include <Arduino.h>

//===========================================================================
//======================== Loconet ==========================================
//===========================================================================

#define USE_LOCONET
#define PIN_LOCONET_RX 8
#define PIN_LOCONET_TX 6

//===========================================================================
//======================== Inputs ===========================================
//===========================================================================

#define USE_INPUTS
#define INPUT_COUNT 8
#define INVERT_INPUTS

#define INPUT_1 19 // A5
#define INPUT_2 18 // A4
#define INPUT_3 17 // A3
#define INPUT_4 4
#define INPUT_5 3
#define INPUT_6 2
#define INPUT_7 1
#define INPUT_8 0

// create input pin array
// TODO: Create array in configuration.h with C preprocessor macro
const uint8_t INPUTS[INPUT_COUNT] = {
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7,
    INPUT_8};

//===========================================================================
//======================== LEDs =============================================
//===========================================================================

#define STATUS_LED
#define PIN_STATUS_LED 9

//===========================================================================
//======================== Settings =========================================
//===========================================================================

#ifndef SETTINGS_DEFAULT_MODE_VALUE
    #define SETTINGS_DEFAULT_MODE_VALUE 971
#endif
