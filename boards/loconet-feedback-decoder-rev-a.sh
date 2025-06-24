#!/bin/sh

# Configuration
BOARD_FQDN=MiniCore:avr:328
BOARD_OPTIONS=clock=16MHz_external,BOD=2v7,eeprom=keep,LTO=Os_flto,variant=modelP,bootloader=no_bootloader
BOARD_NAME=BOARD_LOCONET_FEEDBACK_DECODER_REV_A

PROGRAMMER=stk500
# PORT=
SKETCH=./Arcrail

source $(dirname $0)/board.sh
