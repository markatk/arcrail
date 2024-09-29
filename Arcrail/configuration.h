#pragma once

#include <Arduino.h>

//===========================================================================
//======================== Boards ===========================================
//===========================================================================

// uncomment the desired board in the following section or define a custom
// board in the following section
// #define BOARD_ARDUINO_UNO
// #define BOARD_LOCONET_ACCESSORY_DECODER_REV_C
// #define BOARD_LOCONET_ACCESSORY_DECODER_REV_D
// #define BOARD_LOCONET_FEEDBACK_DECODER_REV_A
// #define BOARD_LOCONET_FEEDBACK_DECODER_REV_B
// #define BOARD_LOCONET_FEEDBACK_DECODER_REV_C
// #define BOARD_LOCONET_TURNOUT_DECODER_REV_A
#define BOARD_LCC_DEVELOPMENT_BOARD_REV_A

#ifdef BOARD_ARDUINO_UNO
    #include "src/boards/arduino-uno.h"
#elif defined BOARD_LOCONET_ACCESSORY_DECODER_REV_C
    #include "src/boards/loconet-accessory-decoder-rev-c.h"
#elif defined BOARD_LOCONET_ACCESSORY_DECODER_REV_D
    #include "src/boards/loconet-accessory-decoder-rev-d.h"
#elif defined BOARD_LOCONET_FEEDBACK_DECODER_REV_A
    #include "src/boards/loconet-feedback-decoder-rev-a.h"
#elif defined BOARD_LOCONET_FEEDBACK_DECODER_REV_B
    #include "src/boards/loconet-feedback-decoder-rev-b.h"
#elif defined BOARD_LOCONET_FEEDBACK_DECODER_REV_C
    #include "src/boards/loconet-feedback-decoder-rev-c.h"
#elif defined BOARD_LOCONET_TURNOUT_DECODER_REV_A
    #include "src/boards/loconet-turnout-decoder-rev-a.h"
#elif defined BOARD_LCC_DEVELOPMENT_BOARD_REV_A
    #include "src/boards/lcc-development-board-rev-a.h"
#else
    #error No board defined
#endif

//===========================================================================
//======================== Firmware =========================================
//===========================================================================

// firmware version is a combination of major and minor version
// for example 205 means 2.05 or 2.5
#ifndef FIRMWARE_VERSION
    #define FIRMWARE_VERSION 104
#endif

//===========================================================================
//======================== Loconet ==========================================
//===========================================================================

#ifdef USE_LOCONET
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
//======================== CAN ==============================================
//===========================================================================

#ifdef USE_CAN
    #ifndef PIN_CAN_CS
        #error CAN chip select pin is not defined
    #endif

// #define PIN_CAN_RESET
// #define CAN_INVERT_RESET
// #define PIN_CAN_INT

    #ifndef CAN_RECEIVE_QUEUE_SIZE
        #define CAN_RECEIVE_QUEUE_SIZE 8
    #endif

    #ifndef CAN_TRANSMIT_QUEUE_SIZE
        #define CAN_TRANSMIT_QUEUE_SIZE 8
    #endif

    #ifdef CAN_USE_MCP2515
        #include "src/can/mcp2515.h"
    #else
        #error No can transfer module defined
    #endif
#endif

//===========================================================================
//======================== LCC ==============================================
//===========================================================================

#ifdef USE_LCC
    #ifndef USE_CAN
        #error CAN is required to use LCC
    #endif

    #if !defined(LCC_DEFAULT_NODE_ID_0) || !defined(LCC_DEFAULT_NODE_ID_1) || !defined(LCC_DEFAULT_NODE_ID_2) || !defined(LCC_DEFAULT_NODE_ID_3) || !defined(LCC_DEFAULT_NODE_ID_4) || !defined(LCC_DEFAULT_NODE_ID_5)
        #error Default LCC node id is not defined
    #endif
#endif

//===========================================================================
//======================== Settings =========================================
//===========================================================================

#ifndef SETTINGS_DEFAULT_MODULE_VALUE
    #define SETTINGS_DEFAULT_MODULE_VALUE 1
#endif

#ifndef SETTINGS_DEFAULT_MODE_VALUE
    #define SETTINGS_DEFAULT_MODE_VALUE 0
#endif

//===========================================================================
//======================== Outputs ==========================================
//===========================================================================

#ifdef USE_OUTPUTS
    #ifndef OUTPUT_COUNT
        #error Output count is not defined
    #endif

    // number of outputs is limited because of the settings layout
    #define OUTPUT_COUNT_MAX 20

    #if OUTPUT_COUNT > OUTPUT_COUNT_MAX
        #error Output count must not be greater than OUTPUT_COUNT_MAX
    #endif
#else
    #define OUTPUT_COUNT 0
#endif

//===========================================================================
//======================== Inputs ===========================================
//===========================================================================

#ifdef USE_INPUTS
    #ifndef INPUT_COUNT
        #error Input count is not defined
    #endif

    // number of inputs is limited because of the settings layout
    #define INPUT_COUNT_MAX 20

    #if INPUT_COUNT > INPUT_COUNT_MAX
        #error Input count must not be greater than INPUT_COUNT_MAX
    #endif

// #define INVERT_INPUTS
// #define PULLUP_INPUTS
#else
    #define INPUT_COUNT 0
#endif

//===========================================================================
//======================== LEDs =============================================
//===========================================================================

#ifdef USE_LEDS
    #ifndef LED_COUNT
        #error LED count is not defined
    #endif

// index in the LEDS array NOT the pin number
// #define STATUS_LED 0
#endif

//===========================================================================
//======================== Buttons ==========================================
//===========================================================================

#ifdef USE_BUTTONS
    #ifndef BUTTON_PRESS_COUNT
        #define BUTTON_PRESS_COUNT 200
    #endif

    #ifndef BUTTON_RELEASE_COUNT
        #define BUTTON_RELEASE_COUNT 20
    #endif

    #ifndef BUTTON_COUNT
        #error Button count is not defined
    #endif

// #define BUTTONS_INVERT
// #define BUTTONS_INTERNAL_PULLUP
#else
    #define BUTTON_COUNT 0
#endif
