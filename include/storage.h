#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint32_t boot_counter;
    uint32_t last_state;
    uint32_t last_fault_code;
} DeviceSnapshot;

int storage_write_snapshot(const char *path, const DeviceSnapshot *snapshot);
int storage_read_snapshot(const char *path, DeviceSnapshot *snapshot);
int storage_clear_file(const char *path);

#endif
