#!/bin/sh

# Configuration
BOARD_FQDN=arduino:avr:uno
BOARD_OPTIONS=""
BOARD_NAME=BOARD_ARDUINO_UNO

PROGRAMMER=arduinoasisp
# PORT=
SKETCH=./Arcrail
LIBRARIES=LocoNet

source $(dirname $0)/board.sh
