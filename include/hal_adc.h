#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    HAL_ADC_TEMPERATURE = 0,
    HAL_ADC_VOLTAGE,
    HAL_ADC_CURRENT,
    HAL_ADC_FAN_RPM,
    HAL_ADC_BATTERY
} HalAdcChannel;

int hal_adc_read(HalAdcChannel channel, int32_t *value);

#endif
