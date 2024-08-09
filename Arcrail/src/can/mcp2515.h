#pragma once

#include "../../configuration.h"

#define MCP2515_CANSTAT 0x0E
#define MCP2515_CANCTRL 0x0F

#define MCP2515_MODE_NORMAL 0x00
#define MCP2515_MODE_SLEEP 0x01
#define MCP2515_MODE_LOOPBACK 0x02
#define MCP2515_MODE_LISTEN_ONLY 0x03
#define MCP2515_MODE_CONFIGURATION 0x04

void mcp2515_init();

void mcp2515_reset();

void mcp2515_change_mode(uint8_t mode);

uint8_t mcp2515_read(uint8_t address);

void mcp2515_write(uint8_t address, uint8_t value);
