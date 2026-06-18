#ifndef HAL_H
#define HAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    HAL_TARGET_PC = 0,
    HAL_TARGET_STM32
} HalTarget;

HalTarget hal_get_target(void);

#endif
