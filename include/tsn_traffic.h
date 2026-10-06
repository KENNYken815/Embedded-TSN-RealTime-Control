#ifndef TSN_TRAFFIC_H
#define TSN_TRAFFIC_H

#include "tsn_types.h"

int tsn_frame_build(tsn_frame_t *frame, uint32_t stream_id, uint32_t sequence,
                    uint64_t tx_time_ns, const uint8_t *payload, uint32_t payload_len);

int tsn_frame_validate(const tsn_frame_t *frame);
void tsn_frame_deliver(tsn_frame_t *frame, uint64_t rx_time_ns);

#endif
