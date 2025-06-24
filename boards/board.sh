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

if [ -n "$BOARD_OPTIONS" ]; then
    $BOARD_OPTIONS_ARGS="--board-options $BOARD_OPTIONS"
fi

# Install required libraries (if there are any)
if [ -n $LIBRARIES ]; then
    echo "Installation required libraries..."

    arduino-cli lib install $LIBRARIES
fi

# Compile project
echo "Compiling project..."

arduino-cli compile \
    --clean \
    -b $BOARD_FQDN \
    $BOARD_OPTIONS_ARGS \
    --build-property build.extra_flags=-D$BOARD_NAME \
    $SKETCH

# Check if parameters for upload are specified
if [ -z $PROGRAMMER ]; then
    echo "Programmer is not set. Skipping upload steps."

    exit 0
fi

if [ -z $PORT ]; then
    echo "Port is not set. Skipping upload steps."

    exit 0
fi

# Set fuses by burning the bootloader
echo "Burning fuses and bootloader..."

arduino-cli burn-bootloader \
    --verify \
    -b $BOARD_FQDN \
    -p $PORT \
    -P $PROGRAMMER \
    $BOARD_OPTIONS_ARGS

# Upload project to
echo "Uploading project..."

arduino-cli upload \
    -b $BOARD_FQDN \
    -p $PORT \
    -P $PROGRAMMER \
    $BOARD_OPTIONS_ARGS \
    $SKETCH

