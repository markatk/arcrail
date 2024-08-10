#include "can.h"

#include "../led.h"

#ifdef USE_CAN
    #ifdef PIN_CAN_RESET
void _can_reset(bool value);
    #endif

    #ifdef CAN_USE_MCP2515
        #include "mcp2515.h"

void _handle_message(uint8_t base_address);
    #endif
#endif

void can_init() {
#ifdef USE_CAN
    #ifdef PIN_CAN_RESET
    pinMode(PIN_CAN_RESET, OUTPUT);
    _can_reset(false);

    #endif

    #ifdef CAN_USE_MCP2515
    mcp2515_init();

    mcp2515_write(MCP2515_CNF1, 0x43);
    mcp2515_write(MCP2515_CNF2, 0xE5);
    mcp2515_write(MCP2515_CNF3, 0x83);

    // clear filters
    mcp2515_write(MCP2515_RXF0SIDH, 0x00);
    mcp2515_write(MCP2515_RXF0SIDL, 0x00);
    mcp2515_write(MCP2515_RXF0EID8, 0x00);
    mcp2515_write(MCP2515_RXF0EID0, 0x00);
    mcp2515_write(MCP2515_RXF1SIDH, 0x00);
    mcp2515_write(MCP2515_RXF1SIDL, 0x00);
    mcp2515_write(MCP2515_RXF1EID8, 0x00);
    mcp2515_write(MCP2515_RXF1EID0, 0x00);
    mcp2515_write(MCP2515_RXF2SIDH, 0x00);
    mcp2515_write(MCP2515_RXF2SIDL, 0x00);
    mcp2515_write(MCP2515_RXF2EID8, 0x00);
    mcp2515_write(MCP2515_RXF2EID0, 0x00);
    mcp2515_write(MCP2515_RXF3SIDH, 0x00);
    mcp2515_write(MCP2515_RXF3SIDL, 0x00);
    mcp2515_write(MCP2515_RXF3EID8, 0x00);
    mcp2515_write(MCP2515_RXF3EID0, 0x00);
    mcp2515_write(MCP2515_RXF4SIDH, 0x00);
    mcp2515_write(MCP2515_RXF4SIDL, 0x00);
    mcp2515_write(MCP2515_RXF4EID8, 0x00);
    mcp2515_write(MCP2515_RXF4EID0, 0x00);
    mcp2515_write(MCP2515_RXF5SIDH, 0x00);
    mcp2515_write(MCP2515_RXF5SIDL, 0x00);
    mcp2515_write(MCP2515_RXF5EID8, 0x00);
    mcp2515_write(MCP2515_RXF5EID0, 0x00);
    mcp2515_write(MCP2515_RXM0SIDH, 0x00);
    mcp2515_write(MCP2515_RXM0SIDL, 0x00);
    mcp2515_write(MCP2515_RXM0EID8, 0x00);
    mcp2515_write(MCP2515_RXM0EIDL, 0x00);
    mcp2515_write(MCP2515_RXM1SIDH, 0x00);
    mcp2515_write(MCP2515_RXM1SIDL, 0x00);
    mcp2515_write(MCP2515_RXM1EID8, 0x00);
    mcp2515_write(MCP2515_RXM1EID0, 0x00);

    mcp2515_bit_modify(MCP2515_RXB0_BASE + MCP2515_RXBxCTRL_OFFSET, 0x60, 0x60);
    mcp2515_bit_modify(MCP2515_RXB1_BASE + MCP2515_RXBxCTRL_OFFSET, 0x60, 0x60);

    mcp2515_change_mode(MCP2515_MODE_NORMAL);
    #endif
#endif
}

void can_update() {
#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
    // check for new messages
    uint8_t status = mcp2515_read_status();
    if ((status & MCP2515_STATUS_MASK_RX0IF) != 0) {
        _handle_message(MCP2515_RXB0_BASE);

        // reset message since it was handled
        mcp2515_bit_modify(MCP2515_CANINTF, 0x01, 0x00);
    }

    if ((status & MCP2515_STATUS_MASK_RX1IF) != 0) {
        _handle_message(MCP2515_RXB1_BASE);

        // reset message since it was handled
        mcp2515_bit_modify(MCP2515_CANINTF, 0x02, 0x00);
    }
    #endif
#endif
}

uint8_t can_send_message(uint32_t identifier, uint8_t length, uint8_t *data) {
#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
    uint8_t transmit_buffer = mcp2515_get_empty_transmit_buffer();
    if (transmit_buffer == 0) {
        return CAN_BUSY;
    }

    // copy header into buffer
    // TODO: Write whole buffer in a continuous write
    // TODO: Use LOAD TX command to jump to the first address
    mcp2515_write(transmit_buffer + MCP2515_TXBxSIDH_OFFSET, identifier >> 3);
    mcp2515_write(transmit_buffer + MCP2515_TXBxSIDL_OFFSET, (identifier & 0x07) << 5 | 0x08 | identifier >> 27);
    mcp2515_write(transmit_buffer + MCP2515_TXBxEID8_OFFSET, identifier >> 19);
    mcp2515_write(transmit_buffer + MCP2515_TXBxEID0_OFFSET, identifier >> 11);
    mcp2515_write(transmit_buffer + MCP2515_TXBxDLC_OFFSET, length & 0x0F);

    // copy data into buffer
    for (uint8_t i = 0; i < length; i++) {
        mcp2515_write(transmit_buffer + MCP2515_TXBxDATA_BASE_OFFSET + i, data[i]);
    }

    // mark buffer to be ready for transmit
    mcp2515_write(transmit_buffer + MCP2515_TXBxCTRL_OFFSET, 0x08);

    return CAN_OK;
    #endif
#endif
}

__attribute__((weak)) void can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data) {
}

#ifdef USE_CAN
    #ifdef PIN_CAN_RESET
void _can_reset(bool value) {
        #ifdef CAN_INVERT_RESET
    digitalWrite(PIN_CAN_RESET, value);
        #else
    digitalWrite(PIN_CAN_RESET, !value);
        #endif
}
    #endif

    #ifdef CAN_USE_MCP2515
void _handle_message(uint8_t base_address) {
    uint32_t identifier = 0;

    // read header
    // TODO: Read whole buffer in a continuous write
    // TODO: Use READ RX command to jump to the first address
    uint32_t value = mcp2515_read(base_address + MCP2515_RXBxSIDH_OFFSET);
    identifier |= value << 3;

    value = mcp2515_read(base_address + MCP2515_RXBxSIDL_OFFSET);
    identifier |= value >> 5;

    // check if message contains extended frame
    if ((value & 0x80) != 0) {
        identifier |= value << 27;
    }

    value = mcp2515_read(base_address + MCP2515_RXBxEID8_OFFSET);
    identifier |= value << 19;

    value = mcp2515_read(base_address + MCP2515_RXBxEID0_OFFSET);
    identifier |= value << 11;

    // read data
    uint8_t length = mcp2515_read(base_address + MCP2515_RXBxDLC_OFFSET) & 0x0F;
    if (length > 8) {
        length = 8;
    }

    uint8_t data[8];
    for (uint8_t i = 0; i < length; i++) {
        value = mcp2515_read(base_address + MCP2515_RXBxDATA_BASE_OFFSET + i);
    }

    can_on_message_received(identifier, length, data);
}
    #endif
#endif
