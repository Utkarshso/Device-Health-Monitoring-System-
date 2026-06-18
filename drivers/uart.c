#define _POSIX_C_SOURCE 200809L

#include "uart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/select.h>
#include <unistd.h>
#endif

void uart_init(void)
{
    (void)setvbuf(stdin, NULL, _IOLBF, 0);
    (void)setvbuf(stdout, NULL, _IOLBF, 0);
}

void uart_deinit(void)
{
    (void)fflush(stdout);
}

void uart_write_line(const char *line)
{
    if (line == NULL)
    {
        return;
    }

    (void)printf("%s\n", line);
    (void)fflush(stdout);
}

void uart_writef(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    (void)vprintf(format, args);
    va_end(args);
    (void)fflush(stdout);
}

bool uart_try_read_line(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0U)
    {
        return false;
    }

#ifdef _WIN32
    if (!_kbhit())
    {
        return false;
    }

    size_t index = 0U;
    while (_kbhit())
    {
        const int character = _getch();
        if (character == '\r' || character == '\n')
        {
            break;
        }

        if (index + 1U < buffer_size)
        {
            buffer[index++] = (char)character;
        }
    }

    buffer[index] = '\0';
    return index > 0U;
#else
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    const int ready = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
    if (ready <= 0 || !FD_ISSET(STDIN_FILENO, &read_fds))
    {
        return false;
    }

    if (fgets(buffer, (int)buffer_size, stdin) == NULL)
    {
        return false;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return true;
#endif
}

void uart_clear_screen(void)
{
    (void)printf("\033[2J\033[H");
    (void)fflush(stdout);
}
