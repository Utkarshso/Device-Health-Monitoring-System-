#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

typedef enum
{
    GPIO_PIN_STATUS_LED = 0,
    GPIO_PIN_ALARM_LED,
    GPIO_PIN_BUZZER,
    GPIO_PIN_COUNT
} GpioPin;

void gpio_init(void);
void gpio_write(GpioPin pin, bool level);
bool gpio_read(GpioPin pin);

#endif
