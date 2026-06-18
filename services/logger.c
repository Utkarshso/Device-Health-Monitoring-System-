#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static void logger_format_timestamp(char *buffer, size_t length)
{
    time_t now = time(NULL);
    const struct tm *local_time = localtime(&now);
    if (local_time == NULL)
    {
        (void)snprintf(buffer, length, "00:00:00");
        return;
    }

    (void)strftime(buffer, length, "%H:%M:%S", local_time);
}

const char *logger_level_to_string(MonitorLogLevel level)
{
    switch (level)
    {
        case MONITOR_LOG_INFO:
            return "INFO";
        case MONITOR_LOG_WARNING:
            return "WARNING";
        case MONITOR_LOG_ERROR:
            return "ERROR";
        case MONITOR_LOG_DEBUG:
            return "DEBUG";
        default:
            return "INFO";
    }
}

int logger_init(Logger *logger, const char *path)
{
    if (logger == NULL || path == NULL)
    {
        return -1;
    }

    (void)snprintf(logger->path, sizeof(logger->path), "%s", path);
    logger->file = fopen(path, "a+");
    if (logger->file == NULL)
    {
        return -1;
    }

    ring_buffer_init(&logger->history, logger->history_storage, MONITOR_LOG_HISTORY);
    return 0;
}

void logger_deinit(Logger *logger)
{
    if (logger == NULL)
    {
        return;
    }

    if (logger->file != NULL)
    {
        (void)fclose(logger->file);
        logger->file = NULL;
    }
}

static void logger_write_line(Logger *logger, MonitorLogLevel level, const char *message)
{
    if (logger == NULL || message == NULL)
    {
        return;
    }

    char timestamp[16];
    logger_format_timestamp(timestamp, sizeof(timestamp));

    char line[MONITOR_LOG_LINE_LENGTH];
    (void)snprintf(line, sizeof(line), "%s %s %s", timestamp, logger_level_to_string(level), message);

    if (logger->file != NULL)
    {
        (void)fprintf(logger->file, "%s\n", line);
        (void)fflush(logger->file);
    }

    (void)ring_buffer_push(&logger->history, line);
}

void logger_log(Logger *logger, MonitorLogLevel level, const char *message)
{
    logger_write_line(logger, level, message);
}

void logger_logf(Logger *logger, MonitorLogLevel level, const char *format, ...)
{
    if (logger == NULL || format == NULL)
    {
        return;
    }

    char message[MONITOR_LOG_LINE_LENGTH];
    va_list args;
    va_start(args, format);
    (void)vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    logger_write_line(logger, level, message);
}

void logger_print_history(const Logger *logger, FILE *out)
{
    if (logger == NULL || out == NULL)
    {
        return;
    }

    for (size_t index = 0U; index < ring_buffer_count(&logger->history); ++index)
    {
        const char *entry = ring_buffer_at(&logger->history, index);
        if (entry != NULL)
        {
            (void)fprintf(out, "%s\n", entry);
        }
    }
}
