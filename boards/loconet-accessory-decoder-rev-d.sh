#!/bin/sh

# Configuration
BOARD_CORE=MiniCore:avr
BOARD_FQDN=MiniCore:avr:328
BOARD_OPTIONS=clock=16MHz_external,BOD=2v7,eeprom=keep,LTO=Os_flto,variant=modelPB,bootloader=no_bootloader
BOARD_NAME=BOARD_LOCONET_ACCESSORY_DECODER_REV_D

PROGRAMMER=stk500
# PORT=
SKETCH=./Arcrail
LIBRARIES=LocoNet

source $(dirname $0)/board.sh
