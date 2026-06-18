#include "gpio.h"

static bool gpio_state[GPIO_PIN_COUNT];

void gpio_init(void)
{
    for (int index = 0; index < GPIO_PIN_COUNT; ++index)
    {
        gpio_state[index] = false;
    }
}

void gpio_write(GpioPin pin, bool level)
{
    if (pin < GPIO_PIN_COUNT)
    {
        gpio_state[pin] = level;
    }
}

bool gpio_read(GpioPin pin)
{
    if (pin >= GPIO_PIN_COUNT)
    {
        return false;
    }

    return gpio_state[pin];
}
