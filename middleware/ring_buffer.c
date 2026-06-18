#include "ring_buffer.h"

#include <stdio.h>
#include <string.h>

void ring_buffer_init(RingBuffer *buffer, char (*storage)[MONITOR_LOG_LINE_LENGTH], size_t capacity)
{
    if (buffer == NULL)
    {
        return;
    }

    buffer->entries = storage;
    buffer->capacity = capacity;
    buffer->head = 0U;
    buffer->tail = 0U;
    buffer->count = 0U;

    for (size_t index = 0U; index < capacity; ++index)
    {
        buffer->entries[index][0] = '\0';
    }
}

bool ring_buffer_push(RingBuffer *buffer, const char *line)
{
    if (buffer == NULL || buffer->entries == NULL || buffer->capacity == 0U || line == NULL)
    {
        return false;
    }

    const size_t insert_index = buffer->head;
    (void)snprintf(buffer->entries[insert_index], MONITOR_LOG_LINE_LENGTH, "%s", line);
    buffer->head = (buffer->head + 1U) % buffer->capacity;

    if (buffer->count < buffer->capacity)
    {
        buffer->count++;
    }
    else
    {
        buffer->tail = (buffer->tail + 1U) % buffer->capacity;
    }

    return true;
}

size_t ring_buffer_count(const RingBuffer *buffer)
{
    return (buffer == NULL) ? 0U : buffer->count;
}

const char *ring_buffer_at(const RingBuffer *buffer, size_t index)
{
    if (buffer == NULL || buffer->entries == NULL || index >= buffer->count)
    {
        return NULL;
    }

    const size_t resolved_index = (buffer->tail + index) % buffer->capacity;
    return buffer->entries[resolved_index];
}

void ring_buffer_clear(RingBuffer *buffer)
{
    if (buffer == NULL || buffer->entries == NULL)
    {
        return;
    }

    buffer->head = 0U;
    buffer->tail = 0U;
    buffer->count = 0U;

    for (size_t index = 0U; index < buffer->capacity; ++index)
    {
        buffer->entries[index][0] = '\0';
    }
}
