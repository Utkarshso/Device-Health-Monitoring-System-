#include "fault_manager.h"

#include <stdio.h>

void fault_manager_init(FaultManager *manager)
{
    if (manager == NULL)
    {
        return;
    }

    manager->report.health = HEALTH_NORMAL;
    manager->report.flags.high_temperature = false;
    manager->report.flags.low_voltage = false;
    manager->report.flags.over_current = false;
    manager->report.flags.low_fan_speed = false;
    manager->report.flags.low_battery = false;
}

FaultReport fault_manager_check(const SensorReadings *readings, const MonitorThresholds *thresholds)
{
    FaultReport report = {0};

    if (readings == NULL || thresholds == NULL)
    {
        report.health = HEALTH_CRITICAL;
        return report;
    }

    report.flags.high_temperature = readings->temperature_c > thresholds->temp_limit_c;
    report.flags.low_voltage = readings->voltage_v < thresholds->low_voltage_v;
    report.flags.over_current = readings->current_a > thresholds->high_current_a;
    report.flags.low_fan_speed = readings->fan_rpm < thresholds->low_fan_rpm;
    report.flags.low_battery = readings->battery_pct < thresholds->low_battery_pct;

    if (readings->temperature_c > thresholds->temp_limit_c + 10.0F ||
        readings->voltage_v < thresholds->low_voltage_v - 1.0F ||
        readings->current_a > thresholds->high_current_a + 1.0F ||
        readings->fan_rpm < (thresholds->low_fan_rpm / 2) ||
        readings->battery_pct < (thresholds->low_battery_pct / 2))
    {
        report.health = HEALTH_CRITICAL;
    }
    else if (report.flags.high_temperature || report.flags.low_voltage || report.flags.over_current || report.flags.low_fan_speed || report.flags.low_battery)
    {
        report.health = HEALTH_WARNING;
    }
    else
    {
        report.health = HEALTH_NORMAL;
    }

    return report;
}

const char *fault_manager_health_to_string(HealthStatus health)
{
    switch (health)
    {
        case HEALTH_NORMAL:
            return "NORMAL";
        case HEALTH_WARNING:
            return "WARNING";
        case HEALTH_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

const char *fault_manager_fault_summary(const FaultReport *report)
{
    static char summary[128];

    if (report == NULL)
    {
        return "No fault data";
    }

    if (report->health == HEALTH_NORMAL)
    {
        (void)snprintf(summary, sizeof(summary), "No active faults");
        return summary;
    }

    (void)snprintf(summary, sizeof(summary), "%s%s%s%s%s",
                   report->flags.high_temperature ? "TEMP " : "",
                   report->flags.low_voltage ? "VOLT " : "",
                   report->flags.over_current ? "CURR " : "",
                   report->flags.low_fan_speed ? "FAN " : "",
                   report->flags.low_battery ? "BATT " : "");
    return summary;
}
