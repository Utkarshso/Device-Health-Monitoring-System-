#include "alarm.h"

#include "hal_gpio.h"

void alarm_init(Alarm *alarm)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->enabled = true;
    alarm->alarm_active = false;
    alarm->buzzer_active = false;
    alarm->led_active = false;
    hal_gpio_write(HAL_GPIO_STATUS_LED, false);
    hal_gpio_write(HAL_GPIO_ALARM_LED, false);
    hal_gpio_write(HAL_GPIO_BUZZER, false);
}

void alarm_update(Alarm *alarm, HealthStatus health, const FaultReport *report)
{
    if (alarm == NULL)
    {
        return;
    }

    const bool alarm_active = (health == HEALTH_WARNING || health == HEALTH_CRITICAL) && report != NULL;
    const bool buzzer_active = health == HEALTH_CRITICAL;

    alarm->alarm_active = alarm_active;
    alarm->buzzer_active = buzzer_active;
    alarm->led_active = alarm_active;

    hal_gpio_write(HAL_GPIO_STATUS_LED, health == HEALTH_NORMAL);
    hal_gpio_write(HAL_GPIO_ALARM_LED, alarm_active);
    hal_gpio_write(HAL_GPIO_BUZZER, buzzer_active);
}

bool alarm_is_active(const Alarm *alarm)
{
    return (alarm != NULL) ? alarm->alarm_active : false;
}
