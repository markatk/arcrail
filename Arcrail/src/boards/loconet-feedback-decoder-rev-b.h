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

#define INPUT_1 9
#define INPUT_2 7
#define INPUT_3 10
#define INPUT_4 4
#define INPUT_5 3
#define INPUT_6 2
#define INPUT_7 A0
#define INPUT_8 A1

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

#define USE_LEDS
#define LED_COUNT 1

#define LED_1 5

// create led pin array
// TODO: Create array in configuration.h with C preprocessor macro
const uint8_t LEDS[LED_COUNT] = {
    LED_1};

// index in the LEDS array NOT the pin number
#define STATUS_LED 0

//===========================================================================
//======================== Settings =========================================
//===========================================================================

#ifndef SETTINGS_DEFAULT_MODE_VALUE
    #define SETTINGS_DEFAULT_MODE_VALUE 971
#endif
