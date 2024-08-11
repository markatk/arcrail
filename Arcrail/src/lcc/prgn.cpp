#include "prgn.h"

uint32_t lfsr1;
uint32_t lfsr2;

void prgn_load_node_id(uint8_t *node_id) {
    // load node id as seed
    lfsr1 = ((uint32_t)node_id[0] << 16) | ((uint32_t)node_id[1] << 8) | node_id[2];
    lfsr2 = ((uint32_t)node_id[3] << 16) | ((uint32_t)node_id[4] << 8) | node_id[5];
}

uint16_t prgn_get_alias() {
    // First, form 2^9*val
    uint32_t temp1 = ((lfsr1 << 9) | ((lfsr2 >> 15) & 0x1FF)) & 0xFFFFFF;
    uint32_t temp2 = (lfsr2 << 9) & 0xFFFFFF;

    // add
    lfsr2 = lfsr2 + temp2 + 0x7A4BA9L;
    lfsr1 = lfsr1 + temp1 + 0x1B0CA3L;

    // carry
    lfsr1 = (lfsr1 & 0xFFFFFF) + ((lfsr2 & 0xFF000000) >> 24);
    lfsr2 = lfsr2 & 0xFFFFFF;

    return (lfsr1 ^ lfsr2 ^ (lfsr1 >> 12) ^ (lfsr2 >> 12)) & 0xFFF;
}
