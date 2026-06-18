#include "app.h"

#include "alarm.h"
#include "config.h"
#include "fault_manager.h"
#include "hal_gpio.h"
#include "hal_storage.h"
#include "hal_timer.h"
#include "hal_uart.h"
#include "logger.h"
#include "scheduler.h"
#include "sensor_manager.h"
#include "state_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    MonitorConfig config;
    SensorManager sensor_manager;
    FaultManager fault_manager;
    Logger logger;
    Alarm alarm;
    StateMachine state_machine;
    Scheduler scheduler;
    SchedulerTask task_storage[8U];
    HalWatchdogTimer watchdog;
    HalDeviceSnapshot snapshot;
    SensorReadings latest_readings;
    FaultReport latest_report;
    HealthStatus previous_health;
    SystemState previous_state;
    bool running;
    char log_path[260];
    char eeprom_path[260];
} MonitorApp;

static void app_draw_dashboard(const MonitorApp *app)
{
    if (app == NULL)
    {
        return;
    }

    hal_uart_clear_screen();
    hal_uart_write_line("====================================");
    hal_uart_write_line("");
    hal_uart_write_line("Embedded Device Health Monitor");
    hal_uart_write_line("");
    hal_uart_write_line("====================================");
    hal_uart_write_line("");
    hal_uart_writef("Temperature : %.1f C\n", app->latest_readings.temperature_c);
    hal_uart_writef("Voltage     : %.1f V\n", app->latest_readings.voltage_v);
    hal_uart_writef("Current     : %.1f A\n", app->latest_readings.current_a);
    hal_uart_writef("Fan RPM     : %d\n", app->latest_readings.fan_rpm);
    hal_uart_writef("Battery     : %d %%\n", app->latest_readings.battery_pct);
    hal_uart_writef("System State: %s\n", state_machine_state_to_string(app->state_machine.state));
    hal_uart_writef("Health      : %s\n", fault_manager_health_to_string(app->latest_report.health));
    hal_uart_writef("Faults      : %s\n", fault_manager_fault_summary(&app->latest_report));
    hal_uart_writef("Alarm       : %s\n", alarm_is_active(&app->alarm) ? "ACTIVE" : "INACTIVE");
    hal_uart_write_line("");
    hal_uart_write_line("Commands: help, status, log, config, clear, reset, exit");
}

static void app_print_status(const MonitorApp *app)
{
    if (app == NULL)
    {
        return;
    }

    hal_uart_writef("Temperature: %.1f C\n", app->latest_readings.temperature_c);
    hal_uart_writef("Voltage    : %.1f V\n", app->latest_readings.voltage_v);
    hal_uart_writef("Current    : %.1f A\n", app->latest_readings.current_a);
    hal_uart_writef("Fan RPM    : %d\n", app->latest_readings.fan_rpm);
    hal_uart_writef("Battery    : %d %%\n", app->latest_readings.battery_pct);
    hal_uart_writef("State      : %s\n", state_machine_state_to_string(app->state_machine.state));
    hal_uart_writef("Health     : %s\n", fault_manager_health_to_string(app->latest_report.health));
    hal_uart_writef("Faults     : %s\n", fault_manager_fault_summary(&app->latest_report));
}

static void app_log_cycle_summary(MonitorApp *app)
{
    if (app == NULL)
    {
        return;
    }

    logger_logf(&app->logger,
                MONITOR_LOG_INFO,
                "Temperature=%.1fC Voltage=%.1fV Current=%.1fA Fan=%dRPM Battery=%d%% State=%s Health=%s",
                app->latest_readings.temperature_c,
                app->latest_readings.voltage_v,
                app->latest_readings.current_a,
                app->latest_readings.fan_rpm,
                app->latest_readings.battery_pct,
                state_machine_state_to_string(app->state_machine.state),
                fault_manager_health_to_string(app->latest_report.health));
}

static void task_read_sensors(void *context)
{
    MonitorApp *app = (MonitorApp *)context;
    if (app == NULL)
    {
        return;
    }

    sensor_manager_read(&app->sensor_manager);
    const SensorReadings *readings = sensor_manager_get_readings(&app->sensor_manager);
    if (readings != NULL)
    {
        app->latest_readings = *readings;
    }

    hal_watchdog_pet(&app->watchdog, hal_timer_now_ms());
}

static void task_check_faults(void *context)
{
    MonitorApp *app = (MonitorApp *)context;
    if (app == NULL)
    {
        return;
    }

    const FaultReport report = fault_manager_check(&app->latest_readings, &app->config.thresholds);
    const HealthStatus previous_health = app->latest_report.health;
    const SystemState previous_state = app->state_machine.state;

    app->latest_report = report;
    (void)state_machine_update(&app->state_machine, report.health, &report);
    alarm_update(&app->alarm, report.health, &report);

    if (report.health != previous_health)
    {
        logger_logf(&app->logger,
                    report.health == HEALTH_CRITICAL ? MONITOR_LOG_ERROR : MONITOR_LOG_WARNING,
                    "Health changed to %s (%s)",
                    fault_manager_health_to_string(report.health),
                    fault_manager_fault_summary(&report));
    }

    if (app->state_machine.state != previous_state)
    {
        logger_logf(&app->logger,
                    MONITOR_LOG_INFO,
                    "State transition %s -> %s",
                    state_machine_state_to_string(previous_state),
                    state_machine_state_to_string(app->state_machine.state));
    }
}

static void task_update_alarm(void *context)
{
    MonitorApp *app = (MonitorApp *)context;
    if (app == NULL)
    {
        return;
    }

    alarm_update(&app->alarm, app->latest_report.health, &app->latest_report);
}

static void task_refresh_dashboard(void *context)
{
    MonitorApp *app = (MonitorApp *)context;
    if (app == NULL)
    {
        return;
    }

    app_draw_dashboard(app);
}

static void task_write_log(void *context)
{
    MonitorApp *app = (MonitorApp *)context;
    if (app == NULL)
    {
        return;
    }

    app_log_cycle_summary(app);
}

static void app_show_help(void)
{
    hal_uart_write_line("Commands:");
    hal_uart_write_line("  help   - Show this help");
    hal_uart_write_line("  status - Display current measurements and state");
    hal_uart_write_line("  log    - Print recent log entries");
    hal_uart_write_line("  config - Display loaded thresholds");
    hal_uart_write_line("  clear  - Clear the terminal");
    hal_uart_write_line("  reset  - Reset the monitored device simulation");
    hal_uart_write_line("  exit   - Stop the monitor");
}

static void app_reset(MonitorApp *app)
{
    if (app == NULL)
    {
        return;
    }

    sensor_manager_reset(&app->sensor_manager);
    app->latest_readings = *sensor_manager_get_readings(&app->sensor_manager);
    state_machine_init(&app->state_machine);
    fault_manager_init(&app->fault_manager);
    alarm_init(&app->alarm);
    app->latest_report.health = HEALTH_NORMAL;
    app->latest_report.flags.high_temperature = false;
    app->latest_report.flags.low_voltage = false;
    app->latest_report.flags.over_current = false;
    app->latest_report.flags.low_fan_speed = false;
    app->latest_report.flags.low_battery = false;
    hal_watchdog_pet(&app->watchdog, hal_timer_now_ms());
    logger_log(&app->logger, MONITOR_LOG_INFO, "System reset by CLI");
}

static void app_handle_command(MonitorApp *app, const char *command)
{
    if (app == NULL || command == NULL || command[0] == '\0')
    {
        return;
    }

    if (strcmp(command, "help") == 0)
    {
        app_show_help();
    }
    else if (strcmp(command, "status") == 0)
    {
        app_print_status(app);
    }
    else if (strcmp(command, "log") == 0)
    {
        logger_print_history(&app->logger, stdout);
    }
    else if (strcmp(command, "config") == 0)
    {
        config_print(&app->config);
    }
    else if (strcmp(command, "clear") == 0)
    {
        hal_uart_clear_screen();
        app_draw_dashboard(app);
    }
    else if (strcmp(command, "reset") == 0)
    {
        app_reset(app);
        app_draw_dashboard(app);
    }
    else if (strcmp(command, "exit") == 0)
    {
        app->running = false;
    }
    else
    {
        hal_uart_writef("Unknown command: %s\n", command);
    }
}

static int app_initialize(MonitorApp *app)
{
    if (app == NULL)
    {
        return -1;
    }

    memset(app, 0, sizeof(*app));
    (void)snprintf(app->log_path, sizeof(app->log_path), "logs/monitor.log");
    (void)snprintf(app->eeprom_path, sizeof(app->eeprom_path), "logs/eeprom.bin");

    hal_uart_init();
    hal_gpio_init();
    srand((unsigned int)time(NULL));

    config_set_defaults(&app->config);
    (void)config_load(&app->config, "config/config.ini");

    if (logger_init(&app->logger, app->log_path) != 0)
    {
        hal_uart_write_line("Failed to open log file.");
        return -1;
    }

    sensor_manager_init(&app->sensor_manager);
    fault_manager_init(&app->fault_manager);
    alarm_init(&app->alarm);
    state_machine_init(&app->state_machine);
    scheduler_init(&app->scheduler, app->task_storage, sizeof(app->task_storage) / sizeof(app->task_storage[0]));
    hal_watchdog_init(&app->watchdog, app->config.thresholds.watchdog_timeout_ms);

    app->latest_readings = *sensor_manager_get_readings(&app->sensor_manager);
    app->latest_report.health = HEALTH_NORMAL;
    app->previous_health = HEALTH_NORMAL;
    app->previous_state = SYSTEM_STATE_INIT;
    app->running = true;

    if (hal_storage_read_snapshot(app->eeprom_path, &app->snapshot) != 0)
    {
        app->snapshot.boot_counter = 0U;
        app->snapshot.last_state = (uint32_t)SYSTEM_STATE_INIT;
        app->snapshot.last_fault_code = 0U;
    }
    app->snapshot.boot_counter++;

    const uint64_t now_ms = hal_timer_now_ms();
    (void)scheduler_add_task(&app->scheduler, "read_sensors", task_read_sensors, app, 1000U, now_ms);
    (void)scheduler_add_task(&app->scheduler, "check_faults", task_check_faults, app, 1000U, now_ms);
    (void)scheduler_add_task(&app->scheduler, "update_alarm", task_update_alarm, app, 1000U, now_ms);
    (void)scheduler_add_task(&app->scheduler, "refresh_dashboard", task_refresh_dashboard, app, 2000U, now_ms);
    (void)scheduler_add_task(&app->scheduler, "write_log", task_write_log, app, 5000U, now_ms);

    logger_logf(&app->logger, MONITOR_LOG_INFO, "System started. Boot #%u", app->snapshot.boot_counter);
    app_draw_dashboard(app);
    return 0;
}

static void app_shutdown(MonitorApp *app)
{
    if (app == NULL)
    {
        return;
    }

    app->snapshot.last_state = (uint32_t)app->state_machine.state;
    app->snapshot.last_fault_code = (uint32_t)app->latest_report.health;
    (void)hal_storage_write_snapshot(app->eeprom_path, &app->snapshot);
    logger_log(&app->logger, MONITOR_LOG_INFO, "System stopped");
    logger_deinit(&app->logger);
    hal_uart_deinit();
}

int app_run(void)
{
    MonitorApp app;
    if (app_initialize(&app) != 0)
    {
        return 1;
    }

    char command_buffer[MONITOR_MAX_LINE_LENGTH];
    while (app.running)
    {
        const uint64_t now_ms = hal_timer_now_ms();
        scheduler_run_pending(&app.scheduler, now_ms);

        if (hal_watchdog_expired(&app.watchdog, now_ms))
        {
            logger_log(&app.logger, MONITOR_LOG_ERROR, "Watchdog expired");
            app.latest_report.health = HEALTH_CRITICAL;
            app.latest_report.flags.high_temperature = true;
            (void)state_machine_update(&app.state_machine, HEALTH_CRITICAL, &app.latest_report);
            alarm_update(&app.alarm, HEALTH_CRITICAL, &app.latest_report);
            hal_watchdog_pet(&app.watchdog, now_ms);
        }

        if (hal_uart_try_read_line(command_buffer, sizeof(command_buffer)))
        {
            app_handle_command(&app, command_buffer);
        }

        hal_timer_sleep_ms(50U);
    }

    app_shutdown(&app);
    return 0;
}
