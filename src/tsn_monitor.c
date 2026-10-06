#include "tsn_monitor.h"
#include <limits.h>

void tsn_monitor_init(tsn_monitor_t *monitor) {
    if (!monitor) return;
    monitor->samples = 0;
    monitor->min_latency_ns = UINT64_MAX;
    monitor->max_latency_ns = 0;
    monitor->total_latency_ns = 0;
    monitor->worst_abs_jitter_ns = 0;
    monitor->deadline_misses = 0;
    monitor->jitter_violations = 0;
}

tsn_latency_sample_t tsn_measure_latency(uint64_t release_ns, uint64_t tx_ns, uint64_t rx_ns,
                                          uint32_t deadline_us, uint32_t max_jitter_us) {
    tsn_latency_sample_t s = {0};
    s.release_time_ns = release_ns;
    s.tx_start_ns = tx_ns;
    s.rx_time_ns = rx_ns;
    s.latency_ns = (rx_ns >= tx_ns) ? (rx_ns - tx_ns) : 0;
    s.jitter_ns = (int64_t)tx_ns - (int64_t)release_ns;
    s.within_deadline = s.latency_ns <= (uint64_t)deadline_us * 1000ULL;
    s.within_jitter = (s.jitter_ns >= -(int64_t)max_jitter_us * 1000LL) &&
                      (s.jitter_ns <= (int64_t)max_jitter_us * 1000LL);
    return s;
}

void tsn_monitor_record(tsn_monitor_t *monitor, const tsn_latency_sample_t *sample) {
    if (!monitor || !sample) return;
    if (monitor->samples == 0) monitor->min_latency_ns = sample->latency_ns;
    if (sample->latency_ns < monitor->min_latency_ns) monitor->min_latency_ns = sample->latency_ns;
    if (sample->latency_ns > monitor->max_latency_ns) monitor->max_latency_ns = sample->latency_ns;
    monitor->total_latency_ns += sample->latency_ns;
    monitor->samples++;
    int64_t abs_jitter = sample->jitter_ns < 0 ? -sample->jitter_ns : sample->jitter_ns;
    if (abs_jitter > monitor->worst_abs_jitter_ns) monitor->worst_abs_jitter_ns = abs_jitter;
    if (!sample->within_deadline) monitor->deadline_misses++;
    if (!sample->within_jitter) monitor->jitter_violations++;
}

uint64_t tsn_monitor_average_latency_ns(const tsn_monitor_t *monitor) {
    if (!monitor || monitor->samples == 0) return 0;
    return monitor->total_latency_ns / monitor->samples;
}

tsn_fault_status_t tsn_monitor_fault_status(const tsn_monitor_t *monitor, uint32_t expected_deadline_us,
                                             uint32_t allowed_jitter_us) {
    tsn_fault_status_t f = {0};
    if (!monitor) {
        f.degraded = true;
        f.clock_sync_faults = 1;
        return f;
    }
    f.deadline_misses = monitor->deadline_misses;
    f.jitter_violations = monitor->jitter_violations;
    f.degraded = (f.deadline_misses > 0) || (f.jitter_violations > 0);
    if (expected_deadline_us == 0 || allowed_jitter_us == 0) f.degraded = true;
    return f;
}
