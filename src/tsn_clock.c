#include "tsn_clock.h"

void tsn_clock_init(tsn_clock_t *clock, uint32_t node_id) {
    if (!clock) return;
    clock->node_id = node_id;
    clock->offset_ns = 0;
    clock->local_time_ns = 0;
    clock->synchronized_time_ns = 0;
    clock->synchronized = false;
}

void tsn_clock_tick(tsn_clock_t *clock, uint64_t elapsed_ns) {
    if (!clock) return;
    clock->local_time_ns += elapsed_ns;
    clock->synchronized_time_ns = (uint64_t)((int64_t)clock->local_time_ns + clock->offset_ns);
}

int tsn_clock_apply_sync(tsn_clock_t *clock, int64_t master_minus_local_ns) {
    if (!clock) return TSN_EINVAL;
    clock->offset_ns = master_minus_local_ns;
    clock->synchronized_time_ns = (uint64_t)((int64_t)clock->local_time_ns + clock->offset_ns);
    clock->synchronized = true;
    return TSN_OK;
}

uint64_t tsn_clock_now(const tsn_clock_t *clock) {
    return clock ? clock->synchronized_time_ns : 0;
}

int64_t tsn_clock_offset(const tsn_clock_t *clock) {
    return clock ? clock->offset_ns : 0;
}
