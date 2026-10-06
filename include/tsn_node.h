#ifndef TSN_NODE_H
#define TSN_NODE_H

#include "tsn_clock.h"
#include "tsn_scheduler.h"
#include "tsn_monitor.h"

typedef struct {
    uint32_t node_id;
    tsn_clock_t clock;
    tsn_scheduler_t scheduler;
    tsn_monitor_t monitor;
    uint32_t tx_sequence;
} tsn_node_t;

void tsn_node_init(tsn_node_t *node, uint32_t node_id);
int tsn_node_sync(tsn_node_t *node, int64_t master_minus_local_ns);
int tsn_node_run_control_cycle(tsn_node_t *node, uint64_t elapsed_ns, uint32_t stream_id,
                               uint64_t simulated_tx_ns, uint64_t simulated_rx_ns);

#endif
