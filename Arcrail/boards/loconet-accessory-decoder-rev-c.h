#ifndef LOCONET_ACCESSORY_DECODER_REV_C_H
#define LOCONET_ACCESSORY_DECODER_REV_C_H

#include "Arduino.h"

//===========================================================================
//======================== Loconet ==========================================
//===========================================================================

#define LOCONET
#define PIN_LOCONET_RX 8
#define PIN_LOCONET_TX 6

//===========================================================================
//======================== Outputs ==========================================
//===========================================================================

#define USE_OUTPUTS
#define OUTPUT_COUNT 16

// outputs 1-16 (no output number 0 because they are labelled on the pcb)
#define OUTPUT_1 9
#define OUTPUT_2 10
#define OUTPUT_3 11
#define OUTPUT_4 12
#define OUTPUT_5 13
#define OUTPUT_6 7
#define OUTPUT_7 A0
#define OUTPUT_8 A1

#define OUTPUT_9 A2
#define OUTPUT_10 A3
#define OUTPUT_11 A4
#define OUTPUT_12 A5
#define OUTPUT_13 3
#define OUTPUT_14 2
#define OUTPUT_15 1
#define OUTPUT_16 0

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
    OUTPUT_16};

//===========================================================================
//======================== LEDs =============================================
//===========================================================================

#define STATUS_LED
#define PIN_STATUS_LED 0

#endif
