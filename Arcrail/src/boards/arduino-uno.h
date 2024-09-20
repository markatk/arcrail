#pragma once

#include "Arduino.h"

//===========================================================================
//======================== Loconet ==========================================
//===========================================================================

#define USE_LOCONET
#define PIN_LOCONET_RX 8
#define PIN_LOCONET_TX 6

//===========================================================================
//======================== Outputs ==========================================
//===========================================================================

#define USE_OUTPUTS
#define OUTPUT_COUNT 11

#define OUTPUT_1 0
#define OUTPUT_2 1
#define OUTPUT_3 2
#define OUTPUT_4 3
#define OUTPUT_5 4
#define OUTPUT_6 5
#define OUTPUT_7 7
#define OUTPUT_8 9
#define OUTPUT_9 10
#define OUTPUT_10 11
#define OUTPUT_11 12

// create output pin array
// TODO: Create array in configuration.h with C preprocessor macro
const uint8_t OUTPUTS[OUTPUT_COUNT] = {
    OUTPUT_1,
    OUTPUT_2,
    OUTPUT_3,
    OUTPUT_4,
    OUTPUT_5,
    OUTPUT_6,
    OUTPUT_7,
    OUTPUT_8,
    OUTPUT_9,
    OUTPUT_10,
    OUTPUT_11};

//===========================================================================
//======================== Inputs ===========================================
//===========================================================================

#define USE_INPUTS
#define INPUT_COUNT 6

#define INPUT_1 A0
#define INPUT_2 A1
#define INPUT_3 A2
#define INPUT_4 A3
#define INPUT_5 A4
#define INPUT_6 A5

// create input pin array
// TODO: Create array in configuration.h with C preprocessor macro
const uint8_t INPUTS[INPUT_COUNT] = {
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6};

//===========================================================================
//======================== LEDs =============================================
//===========================================================================

#define USE_LEDS
#define LED_COUNT 1

#define LED_1 13

// create led pin array
// TODO: Create array in configuration.h with C preprocessor macro
const uint8_t LEDS[LED_COUNT] = {
    LED_1};

// index in the LEDS array NOT the pin number
#define STATUS_LED 0
