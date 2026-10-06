#ifndef TSN_SCHEDULER_H
#define TSN_SCHEDULER_H

#include "tsn_types.h"

#define TSN_MAX_SCHEDULE_ENTRIES 16u

typedef struct {
    tsn_schedule_entry_t entries[TSN_MAX_SCHEDULE_ENTRIES];
    uint32_t count;
} tsn_scheduler_t;

void tsn_scheduler_init(tsn_scheduler_t *scheduler);
int tsn_scheduler_add(tsn_scheduler_t *scheduler, tsn_schedule_entry_t entry);
const tsn_schedule_entry_t *tsn_scheduler_find(const tsn_scheduler_t *scheduler, uint32_t stream_id);
bool tsn_scheduler_is_released(const tsn_schedule_entry_t *entry, uint64_t synchronized_time_us);
uint64_t tsn_scheduler_next_release_us(const tsn_schedule_entry_t *entry, uint64_t synchronized_time_us);

#endif
