#include "mcp2515.h"

#include "../led.h"

#ifdef CAN_USE_MCP2515
    #include <SPI.h>

void _spi_select();
void _spi_unselect();
void _begin_cmd();
void _end_cmd();

void mcp2515_init() {
    pinMode(PIN_CAN_CS, OUTPUT);
    SPI.begin();

    mcp2515_reset();
}

void mcp2515_reset() {
    _begin_cmd();

    SPI.transfer(0xC0);

    _end_cmd();
}

void mcp2515_change_mode(uint8_t mode) {
    // request mode change
    mcp2515_write(MCP2515_CANCTRL, mode << 5);

    // wait till mode is switched
    uint8_t status;

    do {
        status = mcp2515_read(MCP2515_CANSTAT) >> 5;
    } while (status != mode);
}

uint8_t mcp2515_read(uint8_t address) {
    _begin_cmd();

    SPI.transfer(0x03);
    SPI.transfer(address);
    uint8_t value = SPI.transfer(0xFF);

    _end_cmd();

    return value;
}

void mcp2515_read(uint8_t address, uint8_t length, uint8_t *data) {
    _begin_cmd();

    SPI.transfer(0x03);
    SPI.transfer(address);

    for (uint8_t i = 0; i < length; i++) {
        data[i] = SPI.transfer(0xFF); // send data is ignored
    }

    _end_cmd();
}

void mcp2515_write(uint8_t address, uint8_t value) {
    _begin_cmd();

    SPI.transfer(0x02);
    SPI.transfer(address);
    SPI.transfer(value);

    _end_cmd();
}

void mcp2515_write(uint8_t address, uint8_t length, uint8_t *data) {
    _begin_cmd();

    SPI.transfer(0x02);
    SPI.transfer(address);

    for (uint8_t i = 0; i < length; i++) {
        SPI.transfer(data[i]);
    }

    _end_cmd();
}

uint8_t mcp2515_read_status() {
    _begin_cmd();

    SPI.transfer(0xA0);
    uint8_t value = SPI.transfer(0xFF);

    _end_cmd();

    return value;
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t value) {
    _begin_cmd();

    SPI.transfer(0x05);
    SPI.transfer(address);
    SPI.transfer(mask);
    SPI.transfer(value);

    _end_cmd();
}

uint8_t mcp2515_get_empty_transmit_buffer() {
    // check each of the 3 transmit buffers
    uint8_t status = mcp2515_read_status();
    if ((status & MCP2515_STATUS_MASK_TX0REQ) == 0) {
        return MCP2515_TXB0_BASE;
    }

    if ((status & MCP2515_STATUS_MASK_TX1REQ) == 0) {
        return MCP2515_TXB1_BASE;
    }

    if ((status & MCP2515_STATUS_MASK_TX2REQ) == 0) {
        return MCP2515_TXB2_BASE;
    }

    return 0;
}

void _spi_select() {
    digitalWrite(PIN_CAN_CS, LOW);
}

void _spi_unselect() {
    digitalWrite(PIN_CAN_CS, HIGH);
}

void _begin_cmd() {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    _spi_select();
}

void _end_cmd() {
    _spi_unselect();

    SPI.endTransaction();
}
#endif
