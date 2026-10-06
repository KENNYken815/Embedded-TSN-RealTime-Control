#include "tsn_scheduler.h"

void tsn_scheduler_init(tsn_scheduler_t *scheduler) {
    if (scheduler) scheduler->count = 0;
}

int tsn_scheduler_add(tsn_scheduler_t *scheduler, tsn_schedule_entry_t entry) {
    if (!scheduler || entry.period_us == 0 || entry.deadline_us == 0 ||
        entry.max_jitter_us == 0) return TSN_EINVAL;
    if (scheduler->count >= TSN_MAX_SCHEDULE_ENTRIES) return TSN_EFULL;
    scheduler->entries[scheduler->count++] = entry;
    return TSN_OK;
}

const tsn_schedule_entry_t *tsn_scheduler_find(const tsn_scheduler_t *scheduler, uint32_t stream_id) {
    if (!scheduler) return NULL;
    for (uint32_t i = 0; i < scheduler->count; ++i)
        if (scheduler->entries[i].stream_id == stream_id) return &scheduler->entries[i];
    return NULL;
}

bool tsn_scheduler_is_released(const tsn_schedule_entry_t *entry, uint64_t synchronized_time_us) {
    if (!entry || entry->period_us == 0) return false;
    return (synchronized_time_us % entry->period_us) == entry->phase_us % entry->period_us;
}

uint64_t tsn_scheduler_next_release_us(const tsn_schedule_entry_t *entry, uint64_t synchronized_time_us) {
    if (!entry || entry->period_us == 0) return 0;
    const uint64_t period = entry->period_us;
    const uint64_t phase = entry->phase_us % period;
    const uint64_t cycle = synchronized_time_us / period;
    uint64_t release = cycle * period + phase;
    if (release < synchronized_time_us) release += period;
    return release;
}
