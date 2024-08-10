#pragma once

#include <Arduino.h>

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
#define OUTPUT_COUNT 20

// outputs 1-20 (no output number 0 because they are labelled on the pcb)
#define OUTPUT_1 10
#define OUTPUT_2 11
#define OUTPUT_3 12
#define OUTPUT_4 13
#define OUTPUT_5 25
#define OUTPUT_6 26
#define OUTPUT_7 14
#define OUTPUT_8 15
#define OUTPUT_9 16
#define OUTPUT_10 17
#define OUTPUT_11 18
#define OUTPUT_12 19
#define OUTPUT_13 0
#define OUTPUT_14 1
#define OUTPUT_15 2
#define OUTPUT_16 7
#define OUTPUT_17 3
#define OUTPUT_18 4
#define OUTPUT_19 23
#define OUTPUT_20 24

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
    OUTPUT_11,
    OUTPUT_12,
    OUTPUT_13,
    OUTPUT_14,
    OUTPUT_15,
    OUTPUT_16,
    OUTPUT_17,
    OUTPUT_18,
    OUTPUT_19,
    OUTPUT_20};

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
    #define SETTINGS_DEFAULT_MODE_VALUE 1001
#endif
