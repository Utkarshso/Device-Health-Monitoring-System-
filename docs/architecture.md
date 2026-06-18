# Architecture Overview

The project is structured like production embedded firmware that will later be ported to an STM32 target.

## Layers

- Application Layer: `app/` owns startup, CLI handling, dashboard rendering, and orchestration.
- Service Layer: `services/` contains monitoring logic such as sensor simulation, fault analysis, logging, alarm control, and configuration loading.
- Driver Layer: `drivers/` exposes desktop-hosted hardware abstractions for UART, timer, GPIO, and EEPROM-like storage.
- Middleware Layer: `middleware/` contains reusable platform-neutral support code including the cooperative scheduler, state machine, ring buffer, and CRC utility.

## Runtime Model

The application runs a cooperative event loop. The scheduler invokes periodic tasks for sensor sampling, fault evaluation, alarm updates, dashboard refresh, and log emission. A UART-like CLI reads commands from standard input while the monitor loop continues to run.

## Persistence

The logger writes structured text to `logs/monitor.log`. A small file-backed snapshot is used to simulate EEPROM persistence.

## Porting Notes

The code avoids desktop-only abstractions in the higher layers and keeps hardware access behind the driver layer, which makes STM32 porting straightforward.
