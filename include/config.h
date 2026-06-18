#ifndef CONFIG_H
#define CONFIG_H

#include "monitor_types.h"

typedef struct
{
    MonitorThresholds thresholds;
} MonitorConfig;

int config_load(MonitorConfig *config, const char *path);
void config_set_defaults(MonitorConfig *config);
void config_print(const MonitorConfig *config);

#endif
