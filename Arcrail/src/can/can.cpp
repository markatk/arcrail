#include "can.h"

#include "../led.h"

#ifdef USE_CAN
    #include "message_queue.h"

const uint8_t zero_data[12] = {0x00};
uint8_t _transmit_buffer[14] = {0x00};
uint8_t _receive_buffer[5] = {0x00};
can_message_t _receive_message;

can_message_t _receive_queue_data[CAN_RECEIVE_QUEUE_SIZE];
can_message_t _transmit_queue_data[CAN_TRANSMIT_QUEUE_SIZE];

can_message_queue_t _receive_queue = {
    .data = _receive_queue_data,
};

can_message_queue_t _transmit_queue = {
    .data = _transmit_queue_data,
};

    #ifdef PIN_CAN_RESET
void _can_reset(bool value);
    #endif

    #ifdef CAN_USE_MCP2515
        #include "mcp2515.h"

void _send_message(uint8_t buffer_address, uint32_t header, uint8_t length, uint8_t *data);
void _enqueue_message(uint8_t base_address);
void _can_interrupt();
    #endif
#endif

void can_init() {
#ifdef USE_CAN
    #ifdef PIN_CAN_RESET
    pinMode(PIN_CAN_RESET, OUTPUT);
    _can_reset(false);
    #endif

    #ifdef PIN_CAN_INT
        #ifdef CAN_USE_MCP2515
    pinMode(PIN_CAN_INT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_CAN_INT), _can_interrupt, FALLING);
        #else
            #error Interrupt pin is not supported for can implementation
        #endif
    #endif

    message_queue_init(&_receive_queue, CAN_RECEIVE_QUEUE_SIZE);
    message_queue_init(&_transmit_queue, CAN_TRANSMIT_QUEUE_SIZE);

    #ifdef CAN_USE_MCP2515
    mcp2515_init();

        #ifdef PIN_CAN_INT
    const uint8_t config_data[4] = {0x83, 0xE5, 0x43, 0x03};
        #else
    const uint8_t config_data[4] = {0x83, 0xE5, 0x43, 0x00};
        #endif

    mcp2515_write(MCP2515_CNF3, 4, (uint8_t *)config_data);

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
    #if defined(CAN_USE_MCP2515) && !defined(PIN_CAN_INT)
    // check for new messages
    uint8_t status = mcp2515_read_status();
    if ((status & MCP2515_STATUS_MASK_RX0IF) != 0) {
        _enqueue_message(MCP2515_RXB0_BASE);

        // reset message since it was handled
        mcp2515_bit_modify(MCP2515_CANINTF, 0x01, 0x00);
    }

    if ((status & MCP2515_STATUS_MASK_RX1IF) != 0) {
        _enqueue_message(MCP2515_RXB1_BASE);

        // reset message since it was handled
        mcp2515_bit_modify(MCP2515_CANINTF, 0x02, 0x00);
    }
    #endif

    // send message from transmit queue
    can_message_t message;

    if (message_queue_is_empty(&_transmit_queue) == false) {
    #ifdef CAN_USE_MCP2515
        // check if message can be send
        uint8_t transmit_buffer_address = mcp2515_get_empty_transmit_buffer();

        if (transmit_buffer_address > 0 && message_queue_pop(&_transmit_queue, &message)) {
            _send_message(transmit_buffer_address, message.header, message.length, message.data);
        }
    #else
        #error Message send is not implemented
    #endif
    }

    // read message from receive queue
    if (message_queue_pop(&_receive_queue, &message)) {
        can_on_message_received(message.header, message.length, message.data);
    }
#endif
}

void can_send_message(uint32_t identifier, uint8_t length, uint8_t *data) {
#ifdef USE_CAN
    can_message_t message;
    message.header = identifier;
    message.length = length;

    for (uint8_t i = 0; i < length; i++) {
        message.data[i] = data[i];
    }

    if (message_queue_push(&_transmit_queue, message)) {
        // TODO: Increase transmit error counter
    }
#endif
}

bool can_is_transmit_buffer_full() {
#ifdef USE_CAN
    return message_queue_is_full(&_transmit_queue);
#else
    return false;
#endif
}

__attribute__((weak)) void
can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data) {
}

#ifdef USE_CAN
void _send_message(uint8_t buffer_address, uint32_t header, uint8_t length, uint8_t *data) {
    // copy message into buffer
    _transmit_buffer[0] = header >> 3;
    _transmit_buffer[1] = (header & 0x07) << 5 | 0x08 | header >> 27;
    _transmit_buffer[2] = header >> 19;
    _transmit_buffer[3] = header >> 11;
    _transmit_buffer[4] = length & 0x0F;

    for (uint8_t i = 0; i < length; i++) {
        _transmit_buffer[5 + i] = data[i];
    }

    // copy data into buffer
    mcp2515_write(buffer_address + MCP2515_RXBxSIDH_OFFSET, length + 5, _transmit_buffer);

    // mark buffer to be ready for transmit
    mcp2515_write(buffer_address + MCP2515_TXBxCTRL_OFFSET, 0x08);
}

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
void _enqueue_message(uint8_t base_address) {
    // read header into buffer
    mcp2515_read(base_address + MCP2515_RXBxSIDH_OFFSET, 5, _receive_buffer);

    _receive_message.header = _receive_buffer[0] << 3 | _receive_buffer[1] >> 5;

    // check if message contains extended frame
    if ((_receive_buffer[1] & 0x08) != 0) {
        _receive_message.header |= (uint32_t)(_receive_buffer[1] & 0x03) << 27 | (uint32_t)_receive_buffer[2] << 19 | (uint32_t)_receive_buffer[3] << 11;
    }

    // read data
    _receive_message.length = _receive_buffer[4];
    if (_receive_message.length > 8) {
        _receive_message.length = 8;
    }

    mcp2515_read(base_address + MCP2515_RXBxDATA_BASE_OFFSET, _receive_message.length, _receive_message.data);

    message_queue_push(&_receive_queue, _receive_message);
}

void _can_interrupt() {
    uint8_t status = mcp2515_read_status();
    if ((status & MCP2515_STATUS_MASK_RX0IF) != 0) {
        _enqueue_message(MCP2515_RXB0_BASE);

        // reset message since it was handled
        mcp2515_bit_modify(MCP2515_CANINTF, 0x01, 0x00);
    }

    if ((status & MCP2515_STATUS_MASK_RX1IF) != 0) {
        _enqueue_message(MCP2515_RXB1_BASE);

        // reset message since it was handled
        mcp2515_bit_modify(MCP2515_CANINTF, 0x02, 0x00);
    }
}
    #endif
#endif
