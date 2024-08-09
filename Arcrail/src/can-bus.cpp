#include "can-bus.h"

#ifdef USE_CAN
    #include "mcp2515.h"
    #include <SPI.h>

void _spi_select();
void _spi_unselect();
void _begin_cmd();
void _end_cmd();

void _cmd_reset();
uint8_t _cmd_read(uint8_t address);
void _cmd_write(uint8_t address, uint8_t value);
#endif

void can_init() {
#ifdef USE_CAN
    pinMode(PIN_CAN_CS, OUTPUT);
    SPI.begin();

    _cmd_reset();

    // change into normal mode
    _cmd_write(MCP2515_CANCTRL, 0x00);

    // wait for operation mode change
    uint8_t canstat;

    do {
        canstat = _cmd_read(MCP2515_CANSTAT);
    } while (canstat & 0xE0 != 0);
#endif
}

void can_update() {
}

#ifdef USE_CAN
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

void _cmd_reset() {
    _begin_cmd();

    SPI.transfer(0xC0);

    _end_cmd();
}

uint8_t _cmd_read(uint8_t address) {
    _begin_cmd();

    SPI.transfer(0x03);
    SPI.transfer(address);
    uint8_t value = SPI.transfer(0xFF);

    _end_cmd();

    return value;
}

void _cmd_write(uint8_t address, uint8_t value) {
    _begin_cmd();

    SPI.transfer(0x02);
    SPI.transfer(address);
    SPI.transfer(value);

    _end_cmd();
}
#endif
