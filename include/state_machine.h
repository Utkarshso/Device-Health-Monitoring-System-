#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "monitor_types.h"

typedef struct
{
    SystemState state;
    unsigned int recovery_counter;
} StateMachine;

void state_machine_init(StateMachine *machine);
SystemState state_machine_update(StateMachine *machine, HealthStatus health, const FaultReport *report);
const char *state_machine_state_to_string(SystemState state);

#endif
