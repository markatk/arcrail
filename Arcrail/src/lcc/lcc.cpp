#include "lcc.h"

#ifdef USE_LCC
    #include "../can/can.h"
    #include "callbacks.h"
    #include "data_link.h"

void process_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data);
#endif

void lcc_init() {
    lcc_reset();
}

void lcc_update() {
    data_link_update();
}

void lcc_reset() {
    data_link_reset();
}

#ifdef USE_LCC
void can_on_message_received(uint32_t identifier, uint8_t length, uint8_t *data) {
    // msb must be set for lcc messages
    if ((identifier & 0x10000000) == 0) {
        return;
    }

    uint16_t content_field = (identifier & 0x07FFF000) >> 12;
    uint16_t source_nid = identifier & 0x0FFF;

    // handle can control or lcc messages
    if ((identifier & 0x08000000) == 0) {
        data_link_process_message(content_field, source_nid, length, data);
    } else {
        process_message(content_field, source_nid, length, data);
    }
}

void process_message(uint16_t content_field, uint16_t source_nid, uint8_t length, uint8_t *data) {
    lcc_on_message(content_field, source_nid, length, data);
}
#endif
