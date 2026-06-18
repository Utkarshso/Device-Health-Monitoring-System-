# Embedded Device Health Monitoring System

A C17 desktop simulation of embedded firmware that demonstrates a production-oriented, layered firmware architecture for monitoring device health. The code runs on a PC for rapid development and is structured so the same high-level code can be retargeted to microcontrollers (STM32) by replacing the HAL implementation.

Why this project exists
- Teaches a practical, interview-friendly approach to embedded firmware: layered design, HAL boundary, cooperative scheduling, and fault handling.
- Provides a runnable desktop simulation so you can demonstrate behavior without hardware.

Who this is for
- Embedded software engineers preparing for interviews.
- Engineers learning how to structure firmware for portability and testability.
- Educators demonstrating embedded architecture patterns.

Key features
- Layered architecture (Application → Services → Drivers → HAL)
- Simulated sensors: temperature, voltage, current, fan RPM, battery percentage
- Cooperative scheduler and periodic tasks
- Fault detection + simple state machine (NORMAL → WARNING → CRITICAL)
- CLI over stdin/stdout for quick interaction and demos
- Live terminal dashboard and log history (ring buffer)
- File-backed storage simulating EEPROM and CRC checks
- HAL boundary with a PC implementation (`drivers/hal_pc.c`) and an STM32 template
- Unit test harness under `tests/`

Why it helps others
- Reproducible demos: run the full firmware loop on a desktop to show sampling, fault escalation, and alarm behavior.
- Portability: swap the HAL to move from simulation to real hardware with minimal changes.
- Interview material: use this project to explain architecture choices, trade-offs, and how you would map features to an MCU (ISR vs polling, DMA, power states).

Quick demo (build & run)

Requirements: `gcc` (or WSL toolchain) and a POSIX-like shell for the Makefile. On Windows you can use the PowerShell compile fallback below.

Linux / WSL (recommended):

```bash
make
make run
```

Run unit tests:

```bash
make test
```

Windows PowerShell fallback:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
gcc -std=gnu11 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L -Iinclude -Iapp -Idrivers -Iservices -Imiddleware app/main.c app/app.c drivers/hal_adc_pc.c drivers/hal_pc.c drivers/gpio.c drivers/storage.c drivers/timer.c drivers/uart.c middleware/crc.c middleware/ring_buffer.c middleware/scheduler.c middleware/state_machine.c services/alarm.c services/config.c services/fault_manager.c services/logger.c services/sensor_manager.c -o build/embedded_device_monitor.exe
.\build\embedded_device_monitor.exe
```

Project layout (short)

- app/: application entry, scheduler wiring, CLI
- drivers/: platform drivers and HAL implementations
- services/: sensor manager, fault manager, logger, alarm, config
- middleware/: scheduler, ring buffer, CRC, state machine
- include/: public headers and `include/hal_*.h` for the HAL boundary
- tests/: unit tests

How to contribute
- Improve unit tests: add edge-case tests for fault thresholds and state transitions.
- Add STM32 HAL implementation in `drivers/hal_stm32_template.c` to run on hardware.
- Add CI to run `make test` and a simple runtime smoke test.

License
- This repository includes permissive example code you can reuse for learning and demos. Add a `LICENSE` if you want an explicit license for distribution.

Contact / Demo idea
- Use `make run` and type `status` then `log` to show a short live demo. In an interview, explain the HAL swap and how ISR vs polling would change the design.

Enjoy — if you'd like, I can initialize a Git repository here and push it to GitHub for you, or create a polished README badge set and CONTRIBUTING guide.
