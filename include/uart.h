#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stddef.h>

void uart_init(void);
void uart_deinit(void);
void uart_write_line(const char *line);
void uart_writef(const char *format, ...);
bool uart_try_read_line(char *buffer, size_t buffer_size);
void uart_clear_screen(void);

#endif
