#!/bin/sh

# Ensure arduino-cli is installed
arduino-cli version > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "arduino-cli is not installed"

    exit 1
fi

# Ensure required configuration is set
if [ -z $SKETCH ]; then
    echo "No sketch folder provided. Using current folder as sketch folder."

    $SKETCH=.
fi

if [ -z $BOARD_FQDN ]; then
    echo "Board fqdn is not set. Set it with the BOARD_FQDN variable."

    exit 0
fi

if [ -z $BOARD_OPTIONS ]; then
    echo "Board options are not set. Set them with the BOARD_OPTIONS variable."

    exit 0
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

