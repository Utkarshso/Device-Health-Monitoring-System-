#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdbool.h>
#include <stddef.h>

void hal_uart_init(void);
void hal_uart_deinit(void);
void hal_uart_write_line(const char *line);
void hal_uart_writef(const char *format, ...);
bool hal_uart_try_read_line(char *buffer, size_t buffer_size);
void hal_uart_clear_screen(void);

#endif
