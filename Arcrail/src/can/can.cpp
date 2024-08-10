#include "can.h"

#include "../led.h"

#ifdef USE_CAN
const uint8_t zero_data[12] = {0x00};
uint8_t _transmit_buffer[14] = {0x00};
uint8_t _receive_buffer[14] = {0x00};

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

    const uint8_t config_data[3] = {0x83, 0xE5, 0x43};
    mcp2515_write(MCP2515_CNF3, 3, (uint8_t *)config_data);

    // clear filters
    mcp2515_write(MCP2515_RXF0SIDH, 12, (uint8_t *)zero_data);
    mcp2515_write(MCP2515_RXF3SIDH, 12, (uint8_t *)zero_data);
    mcp2515_write(MCP2515_RXM0SIDH, 8, (uint8_t *)zero_data);
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
    uint8_t transmit_buffer_address = mcp2515_get_empty_transmit_buffer();
    if (transmit_buffer_address == 0) {
        return CAN_BUSY;
    }

    // copy message into buffer
    _transmit_buffer[0] = identifier >> 3;
    _transmit_buffer[1] = (identifier & 0x07) << 5 | 0x08 | identifier >> 27;
    _transmit_buffer[2] = identifier >> 19;
    _transmit_buffer[3] = identifier >> 11;
    _transmit_buffer[4] = length & 0x0F;

    for (uint8_t i = 0; i < length; i++) {
        _transmit_buffer[5 + i] = data[i];
    }

    // copy data into buffer
    mcp2515_write(transmit_buffer_address + MCP2515_RXBxSIDH_OFFSET, length + 5, _transmit_buffer);

    // mark buffer to be ready for transmit
    mcp2515_write(transmit_buffer_address + MCP2515_TXBxCTRL_OFFSET, 0x08);

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

    // read header into buffer
    mcp2515_read(base_address + MCP2515_RXBxSIDH_OFFSET, 5, _receive_buffer);

    identifier |= _receive_buffer[0] << 3 | _receive_buffer[1] >> 5;

    // check if message contains extended frame
    if ((_receive_buffer[1] & 0x08) != 0) {
        identifier |= (uint32_t)(_receive_buffer[1] & 0x03) << 27 | (uint32_t)_receive_buffer[2] << 19 | (uint32_t)_receive_buffer[3] << 11;
    }

    // read data
    uint8_t length = _receive_buffer[4];
    if (length > 8) {
        length = 8;
    }

    mcp2515_read(base_address + MCP2515_RXBxDATA_BASE_OFFSET, length, _receive_buffer + 5);

    can_on_message_received(identifier, length, _receive_buffer + 5);
}
    #endif
#endif
