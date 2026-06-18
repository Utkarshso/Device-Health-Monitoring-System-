#include "hal.h"
#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_storage.h"
#include "hal_timer.h"
#include "hal_uart.h"

#include "gpio.h"
#include "storage.h"
#include "timer.h"
#include "uart.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

HalTarget hal_get_target(void)
{
    return HAL_TARGET_PC;
}

void hal_uart_init(void)
{
    uart_init();
}

void hal_uart_deinit(void)
{
    uart_deinit();
}

void hal_uart_write_line(const char *line)
{
    uart_write_line(line);
}

void hal_uart_writef(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    (void)vprintf(format, args);
    (void)fflush(stdout);
    va_end(args);
}

bool hal_uart_try_read_line(char *buffer, size_t buffer_size)
{
    return uart_try_read_line(buffer, buffer_size);
}

void hal_uart_clear_screen(void)
{
    uart_clear_screen();
}

void hal_gpio_init(void)
{
    gpio_init();
}

void hal_gpio_write(HalGpioPin pin, bool level)
{
    gpio_write((GpioPin)pin, level);
}

bool hal_gpio_read(HalGpioPin pin)
{
    return gpio_read((GpioPin)pin);
}

uint64_t hal_timer_now_ms(void)
{
    return timer_now_ms();
}

void hal_timer_sleep_ms(uint32_t delay_ms)
{
    timer_sleep_ms(delay_ms);
}

void hal_watchdog_init(HalWatchdogTimer *watchdog, uint64_t timeout_ms)
{
    watchdog_init((WatchdogTimer *)watchdog, timeout_ms);
}

void hal_watchdog_pet(HalWatchdogTimer *watchdog, uint64_t now_ms)
{
    watchdog_pet((WatchdogTimer *)watchdog, now_ms);
}

bool hal_watchdog_expired(const HalWatchdogTimer *watchdog, uint64_t now_ms)
{
    return watchdog_expired((const WatchdogTimer *)watchdog, now_ms);
}

int hal_storage_write_snapshot(const char *path, const HalDeviceSnapshot *snapshot)
{
    return storage_write_snapshot(path, (const DeviceSnapshot *)snapshot);
}

int hal_storage_read_snapshot(const char *path, HalDeviceSnapshot *snapshot)
{
    return storage_read_snapshot(path, (DeviceSnapshot *)snapshot);
}

int hal_storage_clear_file(const char *path)
{
    return storage_clear_file(path);
}
