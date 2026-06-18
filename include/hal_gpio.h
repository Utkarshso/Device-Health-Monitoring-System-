#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdbool.h>

typedef enum
{
    HAL_GPIO_STATUS_LED = 0,
    HAL_GPIO_ALARM_LED,
    HAL_GPIO_BUZZER,
    HAL_GPIO_COUNT
} HalGpioPin;

void hal_gpio_init(void);
void hal_gpio_write(HalGpioPin pin, bool level);
bool hal_gpio_read(HalGpioPin pin);

#endif
