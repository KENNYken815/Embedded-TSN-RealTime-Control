#ifndef TSN_TYPES_H
#define TSN_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#define TSN_MAX_PAYLOAD 64u
#define TSN_MAX_NODES 8u

typedef enum {
    TSN_STREAM_CONTROL = 0,
    TSN_STREAM_TELEMETRY = 1,
    TSN_STREAM_DIAGNOSTIC = 2
} tsn_stream_type_t;

typedef enum {
    TSN_OK = 0,
    TSN_EINVAL = -1,
    TSN_EFULL = -2,
    TSN_ETIMEOUT = -3,
    TSN_EFAULT = -4
} tsn_status_t;

typedef struct {
    uint32_t node_id;
    int64_t offset_ns;
    uint64_t local_time_ns;
    uint64_t synchronized_time_ns;
    bool synchronized;
} tsn_clock_t;

typedef struct {
    uint32_t stream_id;
    tsn_stream_type_t type;
    uint32_t period_us;
    uint32_t phase_us;
    uint32_t deadline_us;
    uint32_t max_jitter_us;
} tsn_schedule_entry_t;

typedef struct {
    uint32_t stream_id;
    uint32_t sequence;
    uint64_t tx_time_ns;
    uint64_t rx_time_ns;
    uint32_t payload_len;
    uint8_t payload[TSN_MAX_PAYLOAD];
} tsn_frame_t;

typedef struct {
    uint64_t release_time_ns;
    uint64_t tx_start_ns;
    uint64_t rx_time_ns;
    uint64_t latency_ns;
    int64_t jitter_ns;
    bool within_deadline;
    bool within_jitter;
} tsn_latency_sample_t;

typedef struct {
    uint32_t dropped_frames;
    uint32_t deadline_misses;
    uint32_t jitter_violations;
    uint32_t clock_sync_faults;
    bool degraded;
} tsn_fault_status_t;

#endif
