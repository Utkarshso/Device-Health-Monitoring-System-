#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint64_t timeout_ms;
    uint64_t last_pet_ms;
    bool enabled;
} HalWatchdogTimer;

uint64_t hal_timer_now_ms(void);
void hal_timer_sleep_ms(uint32_t delay_ms);
void hal_watchdog_init(HalWatchdogTimer *watchdog, uint64_t timeout_ms);
void hal_watchdog_pet(HalWatchdogTimer *watchdog, uint64_t now_ms);
bool hal_watchdog_expired(const HalWatchdogTimer *watchdog, uint64_t now_ms);

#endif
