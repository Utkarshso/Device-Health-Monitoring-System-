#include "config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void config_apply_default_thresholds(MonitorConfig *config)
{
    config->thresholds.temp_limit_c = 70.0F;
    config->thresholds.low_voltage_v = 10.0F;
    config->thresholds.high_current_a = 5.0F;
    config->thresholds.low_fan_rpm = 1000;
    config->thresholds.low_battery_pct = 20;
    config->thresholds.watchdog_timeout_ms = 4000U;
}

void config_set_defaults(MonitorConfig *config)
{
    if (config == NULL)
    {
        return;
    }

    config_apply_default_thresholds(config);
}

static void trim_line(char *line)
{
    char *start = line;
    while (*start != '\0' && isspace((unsigned char)*start) != 0)
    {
        ++start;
    }

    if (start != line)
    {
        (void)memmove(line, start, strlen(start) + 1U);
    }

    size_t length = strlen(line);
    while (length > 0U && isspace((unsigned char)line[length - 1U]) != 0)
    {
        line[length - 1U] = '\0';
        --length;
    }
}

static void apply_key_value(MonitorConfig *config, const char *key, const char *value)
{
    if (strcmp(key, "TEMP_LIMIT") == 0)
    {
        config->thresholds.temp_limit_c = (float)atof(value);
    }
    else if (strcmp(key, "LOW_VOLTAGE") == 0)
    {
        config->thresholds.low_voltage_v = (float)atof(value);
    }
    else if (strcmp(key, "HIGH_CURRENT") == 0)
    {
        config->thresholds.high_current_a = (float)atof(value);
    }
    else if (strcmp(key, "LOW_FAN") == 0)
    {
        config->thresholds.low_fan_rpm = atoi(value);
    }
    else if (strcmp(key, "LOW_BATTERY") == 0)
    {
        config->thresholds.low_battery_pct = atoi(value);
    }
    else if (strcmp(key, "WATCHDOG_TIMEOUT_MS") == 0)
    {
        config->thresholds.watchdog_timeout_ms = (unsigned int)strtoul(value, NULL, 10);
    }
}

int config_load(MonitorConfig *config, const char *path)
{
    if (config == NULL || path == NULL)
    {
        return -1;
    }

    config_apply_default_thresholds(config);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return -1;
    }

    char line[128];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        trim_line(line);
        if (line[0] == '\0' || line[0] == '#' || line[0] == ';')
        {
            continue;
        }

        char *separator = strchr(line, '=');
        if (separator == NULL)
        {
            continue;
        }

        *separator = '\0';
        char *key = line;
        char *value = separator + 1;
        trim_line(key);
        trim_line(value);
        apply_key_value(config, key, value);
    }

    (void)fclose(file);
    return 0;
}

void config_print(const MonitorConfig *config)
{
    if (config == NULL)
    {
        return;
    }

    (void)printf("Configuration\n");
    (void)printf("  TEMP_LIMIT       = %.1f\n", config->thresholds.temp_limit_c);
    (void)printf("  LOW_VOLTAGE      = %.1f\n", config->thresholds.low_voltage_v);
    (void)printf("  HIGH_CURRENT     = %.1f\n", config->thresholds.high_current_a);
    (void)printf("  LOW_FAN          = %d\n", config->thresholds.low_fan_rpm);
    (void)printf("  LOW_BATTERY      = %d\n", config->thresholds.low_battery_pct);
    (void)printf("  WATCHDOG_TIMEOUT  = %u ms\n", config->thresholds.watchdog_timeout_ms);
}
