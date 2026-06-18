#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include "monitor_types.h"

typedef struct
{
    FaultReport report;
} FaultManager;

void fault_manager_init(FaultManager *manager);
FaultReport fault_manager_check(const SensorReadings *readings, const MonitorThresholds *thresholds);
const char *fault_manager_health_to_string(HealthStatus health);
const char *fault_manager_fault_summary(const FaultReport *report);

#endif
