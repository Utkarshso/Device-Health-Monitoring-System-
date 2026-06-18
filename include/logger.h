#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#include "monitor_types.h"
#include "ring_buffer.h"

typedef struct
{
    FILE *file;
    RingBuffer history;
    char history_storage[MONITOR_LOG_HISTORY][MONITOR_LOG_LINE_LENGTH];
    char path[260];
} Logger;

int logger_init(Logger *logger, const char *path);
void logger_deinit(Logger *logger);
void logger_log(Logger *logger, MonitorLogLevel level, const char *message);
void logger_logf(Logger *logger, MonitorLogLevel level, const char *format, ...);
void logger_print_history(const Logger *logger, FILE *out);
const char *logger_level_to_string(MonitorLogLevel level);

#endif
