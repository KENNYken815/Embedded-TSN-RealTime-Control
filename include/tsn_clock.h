#ifndef TSN_CLOCK_H
#define TSN_CLOCK_H

#include "tsn_types.h"

void tsn_clock_init(tsn_clock_t *clock, uint32_t node_id);
void tsn_clock_tick(tsn_clock_t *clock, uint64_t elapsed_ns);
int tsn_clock_apply_sync(tsn_clock_t *clock, int64_t master_minus_local_ns);
uint64_t tsn_clock_now(const tsn_clock_t *clock);
int64_t tsn_clock_offset(const tsn_clock_t *clock);

#endif
