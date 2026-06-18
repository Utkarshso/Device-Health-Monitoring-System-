#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "monitor_types.h"

typedef struct
{
    SensorReadings readings;
    unsigned int sample_count;
    bool fault_injection_enabled;
} SensorManager;

void sensor_manager_init(SensorManager *manager);
void sensor_manager_reset(SensorManager *manager);
void sensor_manager_read(SensorManager *manager);
const SensorReadings *sensor_manager_get_readings(const SensorManager *manager);

#endif
