#pragma once

#include <Arduino.h>

#define LCC_NODE_ID_LENGTH 6
#define LCC_EVENT_ID_LENGTH 8

typedef struct {
    uint8_t data[6];
} lcc_node_id_t;

typedef struct {
    uint8_t data[8];
} lcc_event_id_t;

typedef uint16_t lcc_mti_t;
typedef uint16_t lcc_node_id_alias_t;
