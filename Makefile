CC := gcc
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -O2 -D_POSIX_C_SOURCE=200809L
INCLUDES := -Iinclude -Iapp -Idrivers -Iservices -Imiddleware
BUILD_DIR := build
APP_TARGET := $(BUILD_DIR)/embedded_device_monitor
TEST_TARGET := $(BUILD_DIR)/monitor_tests
APP_SOURCES := \
	app/main.c \
	app/app.c \
	drivers/hal_adc_pc.c \
	drivers/hal_pc.c \
	drivers/gpio.c \
	drivers/storage.c \
	drivers/timer.c \
	drivers/uart.c \
	middleware/crc.c \
	middleware/ring_buffer.c \
	middleware/scheduler.c \
	middleware/state_machine.c \
	services/alarm.c \
	services/config.c \
	services/fault_manager.c \
	services/logger.c \
	services/sensor_manager.c
TEST_SOURCES := \
	tests/test_main.c \
	drivers/hal_adc_pc.c \
	drivers/hal_pc.c \
	drivers/gpio.c \
	drivers/storage.c \
	drivers/timer.c \
	drivers/uart.c \
	middleware/crc.c \
	middleware/ring_buffer.c \
	middleware/scheduler.c \
	middleware/state_machine.c \
	services/alarm.c \
	services/config.c \
	services/fault_manager.c \
	services/logger.c \
	services/sensor_manager.c

.PHONY: all clean test run

all: $(APP_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(APP_TARGET): $(APP_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

run: $(APP_TARGET)
	./$(APP_TARGET)

clean:
	rm -rf $(BUILD_DIR)
