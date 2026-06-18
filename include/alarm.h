#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>

#include "fault_manager.h"

typedef struct
{
    bool enabled;
    bool alarm_active;
    bool buzzer_active;
    bool led_active;
} Alarm;

void alarm_init(Alarm *alarm);
void alarm_update(Alarm *alarm, HealthStatus health, const FaultReport *report);
bool alarm_is_active(const Alarm *alarm);

#endif
