#include "state_machine.h"

void state_machine_init(StateMachine *machine)
{
    if (machine == NULL)
    {
        return;
    }

    machine->state = SYSTEM_STATE_INIT;
    machine->recovery_counter = 0U;
}

SystemState state_machine_update(StateMachine *machine, HealthStatus health, const FaultReport *report)
{
    if (machine == NULL)
    {
        return SYSTEM_STATE_INIT;
    }

    switch (machine->state)
    {
        case SYSTEM_STATE_INIT:
            machine->state = SYSTEM_STATE_MONITORING;
            break;
        case SYSTEM_STATE_MONITORING:
            if (health == HEALTH_WARNING)
            {
                machine->state = SYSTEM_STATE_WARNING;
            }
            else if (health == HEALTH_CRITICAL)
            {
                machine->state = SYSTEM_STATE_FAULT;
                machine->recovery_counter = 0U;
            }
            break;
        case SYSTEM_STATE_WARNING:
            if (health == HEALTH_NORMAL)
            {
                machine->state = SYSTEM_STATE_MONITORING;
            }
            else if (health == HEALTH_CRITICAL)
            {
                machine->state = SYSTEM_STATE_FAULT;
                machine->recovery_counter = 0U;
            }
            break;
        case SYSTEM_STATE_FAULT:
            if (health == HEALTH_NORMAL && report != NULL)
            {
                machine->state = SYSTEM_STATE_RECOVERY;
                machine->recovery_counter = 1U;
            }
            break;
        case SYSTEM_STATE_RECOVERY:
            if (health == HEALTH_NORMAL)
            {
                machine->recovery_counter++;
                if (machine->recovery_counter >= 3U)
                {
                    machine->state = SYSTEM_STATE_MONITORING;
                    machine->recovery_counter = 0U;
                }
            }
            else if (health == HEALTH_WARNING)
            {
                machine->state = SYSTEM_STATE_WARNING;
                machine->recovery_counter = 0U;
            }
            else
            {
                machine->state = SYSTEM_STATE_FAULT;
                machine->recovery_counter = 0U;
            }
            break;
        default:
            machine->state = SYSTEM_STATE_INIT;
            machine->recovery_counter = 0U;
            break;
    }

    return machine->state;
}

const char *state_machine_state_to_string(SystemState state)
{
    switch (state)
    {
        case SYSTEM_STATE_INIT:
            return "INIT";
        case SYSTEM_STATE_MONITORING:
            return "MONITORING";
        case SYSTEM_STATE_WARNING:
            return "WARNING";
        case SYSTEM_STATE_FAULT:
            return "FAULT";
        case SYSTEM_STATE_RECOVERY:
            return "RECOVERY";
        default:
            return "UNKNOWN";
    }
}
