#include "utility.h"

bool compare_node_ids(lcc_node_id_t a, lcc_node_id_t b) {
    for (uint8_t i = 0; i < LCC_NODE_ID_LENGTH; i++) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }

    return true;
}

bool compare_event_ids(lcc_event_id_t a, lcc_event_id_t b) {
    for (uint8_t i = 0; i < LCC_EVENT_ID_LENGTH; i++) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }

    return true;
}
