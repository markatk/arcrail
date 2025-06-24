#!/bin/sh

# Configuration
BOARD_FQDN=MiniCore:avr:328
BOARD_OPTIONS=clock=16MHz_external,BOD=2v7,eeprom=keep,LTO=Os_flto,variant=modelP,bootloader=no_bootloader
BOARD_NAME=BOARD_LOCONET_FEEDBACK_DECODER_REV_C

PROGRAMMER=stk500
# PORT=

SKETCH=./Arcrail

# Ensure arduino-cli is installed
arduino-cli version > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "arduino-cli is not installed"

    exit 1
fi

# Compile project
echo "Compiling project..."

arduino-cli compile \
    --clean \
    -b $BOARD_FQDN \
    --board-options $BOARD_OPTIONS \
    --build-property build.extra_flags=-D$BOARD_NAME \
    $SKETCH

# Check if parameters for upload are specified
if [ -z $PROGRAMMER ]; then
    echo "Programmer is not set. Skipping upload step."

    exit 0
fi

if [ -z $PORT ]; then
    echo "Port is not set. Skipping upload step."

    exit 0
fi

# Set fuses by burning the bootloader
echo "Burning fuses and bootloader..."

arduino-cli burn-bootloader \
    --verify \
    -b $BOARD_FQDN \
    -p $PORT \
    -P $PROGRAMMER \
    --board-options $BOARD_OPTIONS

# Upload project to
echo "Uploading project..."

arduino-cli upload \
    -b $BOARD_FQDN \
    -p $PORT \
    -P $PROGRAMMER \
    --board-options $BOARD_OPTIONS \
    $SKETCH
