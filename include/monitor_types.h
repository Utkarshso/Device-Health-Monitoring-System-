#ifndef MONITOR_TYPES_H
#define MONITOR_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MONITOR_MAX_LINE_LENGTH 160U
#define MONITOR_LOG_HISTORY 64U
#define MONITOR_LOG_LINE_LENGTH 160U

typedef enum
{
    MONITOR_LOG_INFO = 0,
    MONITOR_LOG_WARNING,
    MONITOR_LOG_ERROR,
    MONITOR_LOG_DEBUG
} MonitorLogLevel;

typedef enum
{
    SYSTEM_STATE_INIT = 0,
    SYSTEM_STATE_MONITORING,
    SYSTEM_STATE_WARNING,
    SYSTEM_STATE_FAULT,
    SYSTEM_STATE_RECOVERY
} SystemState;

typedef enum
{
    HEALTH_NORMAL = 0,
    HEALTH_WARNING,
    HEALTH_CRITICAL
} HealthStatus;

typedef struct
{
    float temperature_c;
    float voltage_v;
    float current_a;
    int fan_rpm;
    int battery_pct;
} SensorReadings;

typedef struct
{
    float temp_limit_c;
    float low_voltage_v;
    float high_current_a;
    int low_fan_rpm;
    int low_battery_pct;
    unsigned int watchdog_timeout_ms;
} MonitorThresholds;

typedef struct
{
    bool high_temperature;
    bool low_voltage;
    bool over_current;
    bool low_fan_speed;
    bool low_battery;
} FaultFlags;

typedef struct
{
    HealthStatus health;
    FaultFlags flags;
} FaultReport;

#endif
