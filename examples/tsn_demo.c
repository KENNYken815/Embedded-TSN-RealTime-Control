#include <stdio.h>
#include "tsn_node.h"
#include "tsn_traffic.h"

static void print_faults(const tsn_monitor_t *m) {
    tsn_fault_status_t f = tsn_monitor_fault_status(m, 100, 20);
    printf("faults: deadline_misses=%u jitter_violations=%u degraded=%s\n",
           f.deadline_misses, f.jitter_violations, f.degraded ? "YES" : "NO");
}

int main(void) {
    tsn_node_t node;
    tsn_node_init(&node, 1);

    tsn_schedule_entry_t control = {
        .stream_id = 0x10,
        .type = TSN_STREAM_CONTROL,
        .period_us = 1000,
        .phase_us = 100,
        .deadline_us = 100,
        .max_jitter_us = 20
    };
    tsn_scheduler_add(&node.scheduler, control);
    tsn_node_sync(&node, 2500);

    uint8_t payload[] = {0x11, 0x22, 0x33, 0x44};
    tsn_frame_t frame;
    tsn_frame_build(&frame, control.stream_id, node.tx_sequence, 1100000, payload, sizeof(payload));
    tsn_frame_deliver(&frame, 1107000);

    tsn_node_run_control_cycle(&node, 1100000, control.stream_id, 1100000, 1115000);
    const uint64_t avg = tsn_monitor_average_latency_ns(&node.monitor);

    printf("Embedded TSN real-time control demo\n");
    printf("node=%u sync_offset=%lld ns synchronized_time=%llu ns\n",
           node.node_id, (long long)tsn_clock_offset(&node.clock),
           (unsigned long long)tsn_clock_now(&node.clock));
    printf("stream=0x%X latency=%llu ns avg=%llu ns seq=%u payload=%u bytes\n",
           frame.stream_id,
           (unsigned long long)(frame.rx_time_ns - frame.tx_time_ns),
           (unsigned long long)avg,
           frame.sequence, frame.payload_len);
    print_faults(&node.monitor);
    return 0;
}
