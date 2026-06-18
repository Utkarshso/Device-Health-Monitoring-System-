#define _POSIX_C_SOURCE 200809L

#include "timer.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h>

uint64_t timer_now_ms(void)
{
    return (uint64_t)time(NULL) * 1000ULL;
}

void timer_sleep_ms(uint32_t delay_ms)
{
#ifdef _WIN32
    Sleep(delay_ms);
#else
    struct timespec request;
    request.tv_sec = (time_t)(delay_ms / 1000U);
    request.tv_nsec = (long)((delay_ms % 1000U) * 1000000UL);
    (void)nanosleep(&request, NULL);
#endif
}

void watchdog_init(WatchdogTimer *watchdog, uint64_t timeout_ms)
{
    if (watchdog == NULL)
    {
        return;
    }

    watchdog->timeout_ms = timeout_ms;
    watchdog->last_pet_ms = timer_now_ms();
    watchdog->enabled = true;
}

void watchdog_pet(WatchdogTimer *watchdog, uint64_t now_ms)
{
    if (watchdog == NULL)
    {
        return;
    }

    watchdog->last_pet_ms = now_ms;
}

bool watchdog_expired(const WatchdogTimer *watchdog, uint64_t now_ms)
{
    if (watchdog == NULL || !watchdog->enabled)
    {
        return false;
    }

    return (now_ms - watchdog->last_pet_ms) > watchdog->timeout_ms;
}
