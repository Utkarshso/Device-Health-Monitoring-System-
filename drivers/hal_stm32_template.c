#include "hal.h"
#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_storage.h"
#include "hal_timer.h"
#include "hal_uart.h"

HalTarget hal_get_target(void)
{
    return HAL_TARGET_STM32;
}

void hal_uart_init(void)
{
    /* Replace with STM32 UART init */
}

void hal_uart_deinit(void)
{
}

void hal_uart_write_line(const char *line)
{
    (void)line;
}

void hal_uart_writef(const char *format, ...)
{
    (void)format;
}

bool hal_uart_try_read_line(char *buffer, size_t buffer_size)
{
    (void)buffer;
    (void)buffer_size;
    return false;
}

void hal_uart_clear_screen(void)
{
}

void hal_gpio_init(void)
{
}

void hal_gpio_write(HalGpioPin pin, bool level)
{
    (void)pin;
    (void)level;
}

bool hal_gpio_read(HalGpioPin pin)
{
    (void)pin;
    return false;
}

uint64_t hal_timer_now_ms(void)
{
    return 0U;
}

void hal_timer_sleep_ms(uint32_t delay_ms)
{
    (void)delay_ms;
}

void hal_watchdog_init(HalWatchdogTimer *watchdog, uint64_t timeout_ms)
{
    if (watchdog != NULL)
    {
        watchdog->timeout_ms = timeout_ms;
        watchdog->last_pet_ms = 0U;
        watchdog->enabled = true;
    }
}

void hal_watchdog_pet(HalWatchdogTimer *watchdog, uint64_t now_ms)
{
    if (watchdog != NULL)
    {
        watchdog->last_pet_ms = now_ms;
    }
}

bool hal_watchdog_expired(const HalWatchdogTimer *watchdog, uint64_t now_ms)
{
    if (watchdog == NULL || !watchdog->enabled)
    {
        return false;
    }

    return (now_ms - watchdog->last_pet_ms) > watchdog->timeout_ms;
}

int hal_storage_write_snapshot(const char *path, const HalDeviceSnapshot *snapshot)
{
    (void)path;
    (void)snapshot;
    return 0;
}

int hal_storage_read_snapshot(const char *path, HalDeviceSnapshot *snapshot)
{
    (void)path;
    (void)snapshot;
    return -1;
}

int hal_storage_clear_file(const char *path)
{
    (void)path;
    return 0;
}
