#include "crc.h"
#include "ring_buffer.h"
#include "state_machine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_crc(void)
{
    const uint8_t payload[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    assert(crc16_ccitt(payload, sizeof(payload)) == 0x29B1U);
}

static void test_ring_buffer(void)
{
    char storage[3U][MONITOR_LOG_LINE_LENGTH];
    RingBuffer buffer;
    ring_buffer_init(&buffer, storage, 3U);

    assert(ring_buffer_push(&buffer, "first"));
    assert(ring_buffer_push(&buffer, "second"));
    assert(ring_buffer_push(&buffer, "third"));
    assert(ring_buffer_count(&buffer) == 3U);
    assert(strcmp(ring_buffer_at(&buffer, 0U), "first") == 0);
    assert(strcmp(ring_buffer_at(&buffer, 2U), "third") == 0);

    assert(ring_buffer_push(&buffer, "fourth"));
    assert(ring_buffer_count(&buffer) == 3U);
    assert(strcmp(ring_buffer_at(&buffer, 0U), "second") == 0);
    assert(strcmp(ring_buffer_at(&buffer, 2U), "fourth") == 0);
}

static void test_state_machine(void)
{
    StateMachine machine;
    state_machine_init(&machine);
    assert(machine.state == SYSTEM_STATE_INIT);

    const FaultReport normal_report = { .health = HEALTH_NORMAL };
    const FaultReport warning_report = { .health = HEALTH_WARNING };
    const FaultReport critical_report = { .health = HEALTH_CRITICAL };

    assert(state_machine_update(&machine, HEALTH_NORMAL, &normal_report) == SYSTEM_STATE_MONITORING);
    assert(state_machine_update(&machine, HEALTH_WARNING, &warning_report) == SYSTEM_STATE_WARNING);
    assert(state_machine_update(&machine, HEALTH_CRITICAL, &critical_report) == SYSTEM_STATE_FAULT);
}

int main(void)
{
    test_crc();
    test_ring_buffer();
    test_state_machine();
    puts("All tests passed.");
    return 0;
}
