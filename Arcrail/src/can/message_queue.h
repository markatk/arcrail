#pragma once

#include "../../configuration.h"

typedef struct {
    uint32_t header;
    uint8_t length;
    uint8_t data[8];
} can_message_t;

typedef struct {
    can_message_t *data;
    uint8_t read;
    uint8_t write;
    uint8_t size;
} can_message_queue_t;

void message_queue_init(can_message_queue_t *queue, uint8_t size);

void message_queue_reset(can_message_queue_t *queue);

bool message_queue_push(can_message_queue_t *queue, can_message_t message);

bool message_queue_pop(can_message_queue_t *queue, can_message_t *message);

bool message_queue_peek(can_message_queue_t *queue, can_message_t *message);

bool message_queue_is_empty(can_message_queue_t *queue);

bool message_queue_is_full(can_message_queue_t *queue);

uint8_t message_queue_count(can_message_queue_t *queue);
