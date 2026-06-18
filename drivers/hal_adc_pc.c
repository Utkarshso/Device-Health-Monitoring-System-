#include "hal_adc.h"

#include <stdlib.h>

int hal_adc_read(HalAdcChannel channel, int32_t *value)
{
    if (value == NULL)
    {
        return -1;
    }

    switch (channel)
    {
        case HAL_ADC_TEMPERATURE:
            *value = 350 + (rand() % 90);
            break;
        case HAL_ADC_VOLTAGE:
            *value = 121 + (rand() % 10);
            break;
        case HAL_ADC_CURRENT:
            *value = 14 + (rand() % 8);
            break;
        case HAL_ADC_FAN_RPM:
            *value = 2100 + (rand() % 600);
            break;
        case HAL_ADC_BATTERY:
            *value = 90 + (rand() % 10);
            break;
        default:
            return -1;
    }

    return 0;
}
