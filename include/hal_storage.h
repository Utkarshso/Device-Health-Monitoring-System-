#ifndef HAL_STORAGE_H
#define HAL_STORAGE_H

#include <stdint.h>

typedef struct
{
    uint32_t boot_counter;
    uint32_t last_state;
    uint32_t last_fault_code;
} HalDeviceSnapshot;

int hal_storage_write_snapshot(const char *path, const HalDeviceSnapshot *snapshot);
int hal_storage_read_snapshot(const char *path, HalDeviceSnapshot *snapshot);
int hal_storage_clear_file(const char *path);

#endif
