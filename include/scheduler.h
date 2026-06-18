#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*SchedulerTaskFn)(void *context);

typedef struct
{
    const char *name;
    SchedulerTaskFn function;
    void *context;
    uint32_t period_ms;
    uint64_t next_run_ms;
    bool enabled;
} SchedulerTask;

typedef struct
{
    SchedulerTask *tasks;
    size_t capacity;
    size_t count;
} Scheduler;

void scheduler_init(Scheduler *scheduler, SchedulerTask *tasks, size_t capacity);
bool scheduler_add_task(Scheduler *scheduler, const char *name, SchedulerTaskFn function, void *context, uint32_t period_ms, uint64_t start_time_ms);
void scheduler_run_pending(Scheduler *scheduler, uint64_t now_ms);
void scheduler_enable_all(Scheduler *scheduler, bool enabled);

#endif
