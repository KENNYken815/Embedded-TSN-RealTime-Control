#include "tsn_node.h"

void tsn_node_init(tsn_node_t *node, uint32_t node_id) {
    if (!node) return;
    node->node_id = node_id;
    tsn_clock_init(&node->clock, node_id);
    tsn_scheduler_init(&node->scheduler);
    tsn_monitor_init(&node->monitor);
    node->tx_sequence = 0;
}

int tsn_node_sync(tsn_node_t *node, int64_t master_minus_local_ns) {
    if (!node) return TSN_EINVAL;
    return tsn_clock_apply_sync(&node->clock, master_minus_local_ns);
}

int tsn_node_run_control_cycle(tsn_node_t *node, uint64_t elapsed_ns, uint32_t stream_id,
                               uint64_t simulated_tx_ns, uint64_t simulated_rx_ns) {
    if (!node) return TSN_EINVAL;
    const tsn_schedule_entry_t *entry = tsn_scheduler_find(&node->scheduler, stream_id);
    if (!entry) return TSN_EINVAL;

    tsn_clock_tick(&node->clock, elapsed_ns);

    uint64_t now_us = tsn_clock_now(&node->clock) / 1000ULL;
    uint64_t release_us = tsn_scheduler_next_release_us(entry, now_us);
    uint64_t release_ns = release_us * 1000ULL;

    (void)node->tx_sequence++;
    tsn_latency_sample_t sample =
        tsn_measure_latency(release_ns, simulated_tx_ns, simulated_rx_ns,
                            entry->deadline_us, entry->max_jitter_us);
    tsn_monitor_record(&node->monitor, &sample);
    return sample.within_deadline && sample.within_jitter ? TSN_OK : TSN_EFAULT;
}
