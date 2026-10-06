#ifndef TSN_MONITOR_H
#define TSN_MONITOR_H

#include "tsn_types.h"

typedef struct {
    uint32_t samples;
    uint64_t min_latency_ns;
    uint64_t max_latency_ns;
    uint64_t total_latency_ns;
    int64_t worst_abs_jitter_ns;
    uint32_t deadline_misses;
    uint32_t jitter_violations;
} tsn_monitor_t;

void tsn_monitor_init(tsn_monitor_t *monitor);
void tsn_monitor_record(tsn_monitor_t *monitor, const tsn_latency_sample_t *sample);
uint64_t tsn_monitor_average_latency_ns(const tsn_monitor_t *monitor);
tsn_fault_status_t tsn_monitor_fault_status(const tsn_monitor_t *monitor, uint32_t expected_deadline_us,
                                             uint32_t allowed_jitter_us);
tsn_latency_sample_t tsn_measure_latency(uint64_t release_ns, uint64_t tx_ns, uint64_t rx_ns,
                                          uint32_t deadline_us, uint32_t max_jitter_us);

#endif
