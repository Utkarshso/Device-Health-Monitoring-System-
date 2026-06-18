#include "storage.h"

#include <stdio.h>

int storage_write_snapshot(const char *path, const DeviceSnapshot *snapshot)
{
    if (path == NULL || snapshot == NULL)
    {
        return -1;
    }

    FILE *file = fopen(path, "wb");
    if (file == NULL)
    {
        return -1;
    }

    const size_t written = fwrite(snapshot, sizeof(*snapshot), 1U, file);
    (void)fclose(file);
    return (written == 1U) ? 0 : -1;
}

int storage_read_snapshot(const char *path, DeviceSnapshot *snapshot)
{
    if (path == NULL || snapshot == NULL)
    {
        return -1;
    }

    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        return -1;
    }

    const size_t read_count = fread(snapshot, sizeof(*snapshot), 1U, file);
    (void)fclose(file);
    return (read_count == 1U) ? 0 : -1;
}

int storage_clear_file(const char *path)
{
    if (path == NULL)
    {
        return -1;
    }

    FILE *file = fopen(path, "wb");
    if (file == NULL)
    {
        return -1;
    }

    (void)fclose(file);
    return 0;
}
