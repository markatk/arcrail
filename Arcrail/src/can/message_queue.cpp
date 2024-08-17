#include "message_queue.h"

#ifdef USE_CAN
void message_queue_init(can_message_queue_t *queue, uint8_t size) {
    queue->size = size;

    message_queue_reset(queue);
}

void message_queue_reset(can_message_queue_t *queue) {
    queue->write = 0;
    queue->read = 0;
}

bool message_queue_push(can_message_queue_t *queue, can_message_t message) {
    if (message_queue_is_full(queue)) {
        return false;
    }

    queue->data[queue->write] = message;

    queue->write += 1;
    if (queue->write >= queue->size) {
        queue->write = 0;
    }

    return true;
}

bool message_queue_pop(can_message_queue_t *queue, can_message_t *message) {
    if (message_queue_is_empty(queue)) {
        return false;
    }

    *message = queue->data[queue->read];

    queue->read += 1;
    if (queue->read >= queue->size) {
        queue->read = 0;
    }

    return true;
}

bool message_queue_peek(can_message_queue_t *queue, can_message_t *message) {
    if (message_queue_is_empty(queue)) {
        return false;
    }

    *message = queue->data[queue->read];

    return true;
}

bool message_queue_is_empty(can_message_queue_t *queue) {
    return queue->read == queue->write;
}

bool message_queue_is_full(can_message_queue_t *queue) {
    return (queue->read == queue->write + 1) || (queue->read == 0 && queue->write + 1 == queue->size);
}

uint8_t message_queue_count(can_message_queue_t *queue) {
    if (queue->write >= queue->read) {
        return queue->write - queue->read;
    }

    return queue->size + queue->write - queue->read;
}
#endif
