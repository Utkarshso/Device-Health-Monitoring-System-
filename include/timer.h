#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint64_t timeout_ms;
    uint64_t last_pet_ms;
    bool enabled;
} WatchdogTimer;

uint64_t timer_now_ms(void);
void timer_sleep_ms(uint32_t delay_ms);
void watchdog_init(WatchdogTimer *watchdog, uint64_t timeout_ms);
void watchdog_pet(WatchdogTimer *watchdog, uint64_t now_ms);
bool watchdog_expired(const WatchdogTimer *watchdog, uint64_t now_ms);

#endif
