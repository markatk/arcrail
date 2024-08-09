#pragma once

#include "../../configuration.h"

#define MCP2515_CANSTAT 0x0E
#define MCP2515_CANCTRL 0x0F

#define MCP2515_TXB0_BASE 0x30
#define MCP2515_TXB1_BASE 0x40
#define MCP2515_TXB2_BASE 0x50

#define MCP2515_TXBxCTRL_OFFSET 0
#define MCP2515_TXBxSIDH_OFFSET 1
#define MCP2515_TXBxSIDL_OFFSET 2
#define MCP2515_TXBxEID8_OFFSET 3
#define MCP2515_TXBxEID0_OFFSET 4
#define MCP2515_TXBxDLC_OFFSET 5
#define MCP2515_TXBxDATA_BASE_OFFSET 6

#define MCP2515_MODE_NORMAL 0x00
#define MCP2515_MODE_SLEEP 0x01
#define MCP2515_MODE_LOOPBACK 0x02
#define MCP2515_MODE_LISTEN_ONLY 0x03
#define MCP2515_MODE_CONFIGURATION 0x04

#define MCP2515_TXBnCTRL_MASK_TXREQ 0x08

void mcp2515_init();

void mcp2515_reset();

void mcp2515_change_mode(uint8_t mode);

uint8_t mcp2515_read(uint8_t address);

void mcp2515_write(uint8_t address, uint8_t value);

uint8_t mcp2515_get_empty_transmit_buffer();
