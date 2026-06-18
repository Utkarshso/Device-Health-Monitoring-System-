#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>

#include "monitor_types.h"

typedef struct
{
    char (*entries)[MONITOR_LOG_LINE_LENGTH];
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} RingBuffer;

void ring_buffer_init(RingBuffer *buffer, char (*storage)[MONITOR_LOG_LINE_LENGTH], size_t capacity);
bool ring_buffer_push(RingBuffer *buffer, const char *line);
size_t ring_buffer_count(const RingBuffer *buffer);
const char *ring_buffer_at(const RingBuffer *buffer, size_t index);
void ring_buffer_clear(RingBuffer *buffer);

#endif
