#include "scheduler.h"

bool scheduler_add_task(Scheduler *scheduler, const char *name, SchedulerTaskFn function, void *context, uint32_t period_ms, uint64_t start_time_ms)
{
    if (scheduler == NULL || function == NULL || scheduler->tasks == NULL || scheduler->count >= scheduler->capacity)
    {
        return false;
    }

    SchedulerTask *task = &scheduler->tasks[scheduler->count];
    task->name = name;
    task->function = function;
    task->context = context;
    task->period_ms = period_ms;
    task->next_run_ms = start_time_ms + (uint64_t)period_ms;
    task->enabled = true;
    scheduler->count++;
    return true;
}

void scheduler_init(Scheduler *scheduler, SchedulerTask *tasks, size_t capacity)
{
    if (scheduler == NULL)
    {
        return;
    }

    scheduler->tasks = tasks;
    scheduler->capacity = capacity;
    scheduler->count = 0U;
}

void scheduler_run_pending(Scheduler *scheduler, uint64_t now_ms)
{
    if (scheduler == NULL || scheduler->tasks == NULL)
    {
        return;
    }

    for (size_t index = 0U; index < scheduler->count; ++index)
    {
        SchedulerTask *task = &scheduler->tasks[index];

        if (!task->enabled)
        {
            continue;
        }

        while (now_ms >= task->next_run_ms)
        {
            task->function(task->context);
            task->next_run_ms += (uint64_t)task->period_ms;
        }
    }
}

void scheduler_enable_all(Scheduler *scheduler, bool enabled)
{
    if (scheduler == NULL || scheduler->tasks == NULL)
    {
        return;
    }

    for (size_t index = 0U; index < scheduler->count; ++index)
    {
        scheduler->tasks[index].enabled = enabled;
    }
}
