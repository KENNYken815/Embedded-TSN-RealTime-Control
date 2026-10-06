#ifndef TSN_GCL_H
#define TSN_GCL_H
#include <stdint.h>
#include <stdbool.h>
#define TSN_MAX_GCL_ENTRIES 16u
typedef struct { uint8_t gate_mask; uint32_t interval_us; } tsn_gcl_entry_t;
typedef struct { tsn_gcl_entry_t entries[TSN_MAX_GCL_ENTRIES]; uint32_t count; uint32_t cycle_time_us; } tsn_gcl_t;
void tsn_gcl_init(tsn_gcl_t *gcl);
int tsn_gcl_add(tsn_gcl_t *gcl, uint8_t gate_mask, uint32_t interval_us);
int tsn_gcl_finalize(tsn_gcl_t *gcl);
uint8_t tsn_gcl_gate_at(const tsn_gcl_t *gcl, uint64_t time_us);
#endif
