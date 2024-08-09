#include "can.h"

#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
        #include "mcp2515.h"
    #endif
#endif

void can_init() {
#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
    mcp2515_init();
    mcp2515_change_mode(MCP2515_MODE_NORMAL);
    #endif
#endif
}

void can_update() {
}

uint8_t can_send_message(uint32_t identifier, uint8_t length, uint8_t *data) {
#ifdef USE_CAN
    #ifdef CAN_USE_MCP2515
    uint8_t transmit_buffer = mcp2515_get_empty_transmit_buffer();
    if (transmit_buffer == 0) {
        return CAN_BUSY;
    }

    // copy header into buffer
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
