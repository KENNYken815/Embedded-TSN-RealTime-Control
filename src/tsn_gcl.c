#include "tsn_gcl.h"
void tsn_gcl_init(tsn_gcl_t *gcl) { if (!gcl) return; gcl->count=0; gcl->cycle_time_us=0; }
int tsn_gcl_add(tsn_gcl_t *gcl, uint8_t gate_mask, uint32_t interval_us) {
    if (!gcl || interval_us==0 || gcl->count>=TSN_MAX_GCL_ENTRIES) return -1;
    gcl->entries[gcl->count].gate_mask=gate_mask; gcl->entries[gcl->count].interval_us=interval_us;
    gcl->count++; gcl->cycle_time_us += interval_us; return 0;
}
int tsn_gcl_finalize(tsn_gcl_t *gcl) { return (!gcl || gcl->count==0 || gcl->cycle_time_us==0) ? -1 : 0; }
uint8_t tsn_gcl_gate_at(const tsn_gcl_t *gcl, uint64_t time_us) {
    if (!gcl || gcl->count==0 || gcl->cycle_time_us==0) return 0;
    uint64_t phase=time_us%gcl->cycle_time_us, elapsed=0;
    for (uint32_t i=0;i<gcl->count;++i) { elapsed+=gcl->entries[i].interval_us; if (phase<elapsed) return gcl->entries[i].gate_mask; }
    return gcl->entries[gcl->count-1].gate_mask;
}
