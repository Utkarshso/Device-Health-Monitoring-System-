#include "sensor_manager.h"

#include <stdlib.h>

static float random_float(float minimum, float maximum)
{
    const float scale = (float)rand() / (float)RAND_MAX;
    return minimum + (maximum - minimum) * scale;
}

static int random_int(int minimum, int maximum)
{
    return minimum + (rand() % (maximum - minimum + 1));
}

void sensor_manager_init(SensorManager *manager)
{
    if (manager == NULL)
    {
        return;
    }

    manager->sample_count = 0U;
    manager->fault_injection_enabled = false;
    sensor_manager_reset(manager);
}

void sensor_manager_reset(SensorManager *manager)
{
    if (manager == NULL)
    {
        return;
    }

    manager->readings.temperature_c = 35.0F;
    manager->readings.voltage_v = 12.1F;
    manager->readings.current_a = 1.4F;
    manager->readings.fan_rpm = 2100;
    manager->readings.battery_pct = 91;
    manager->sample_count = 0U;
}

void sensor_manager_read(SensorManager *manager)
{
    if (manager == NULL)
    {
        return;
    }

    manager->sample_count++;

    manager->readings.temperature_c = random_float(31.0F, 39.0F);
    manager->readings.voltage_v = random_float(11.7F, 12.6F);
    manager->readings.current_a = random_float(0.8F, 2.2F);
    manager->readings.fan_rpm = random_int(1900, 2500);
    manager->readings.battery_pct = random_int(78, 100);

    const unsigned int fault_chance = manager->fault_injection_enabled ? 3U : 12U;
    if ((rand() % 100U) < fault_chance)
    {
        switch (rand() % 5U)
        {
            case 0U:
                manager->readings.temperature_c = random_float(72.0F, 92.0F);
                break;
            case 1U:
                manager->readings.voltage_v = random_float(8.0F, 9.8F);
                break;
            case 2U:
                manager->readings.current_a = random_float(5.5F, 8.0F);
                break;
            case 3U:
                manager->readings.fan_rpm = random_int(200, 800);
                break;
            default:
                manager->readings.battery_pct = random_int(5, 15);
                break;
        }
    }
}

const SensorReadings *sensor_manager_get_readings(const SensorManager *manager)
{
    if (manager == NULL)
    {
        return NULL;
    }

    return &manager->readings;
}
