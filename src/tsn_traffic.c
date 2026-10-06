#include "tsn_traffic.h"
#include <string.h>

int tsn_frame_build(tsn_frame_t *frame, uint32_t stream_id, uint32_t sequence,
                    uint64_t tx_time_ns, const uint8_t *payload, uint32_t payload_len) {
    if (!frame || (!payload && payload_len) || payload_len > TSN_MAX_PAYLOAD) return TSN_EINVAL;
    memset(frame, 0, sizeof(*frame));
    frame->stream_id = stream_id;
    frame->sequence = sequence;
    frame->tx_time_ns = tx_time_ns;
    frame->payload_len = payload_len;
    if (payload_len) memcpy(frame->payload, payload, payload_len);
    return TSN_OK;
}

int tsn_frame_validate(const tsn_frame_t *frame) {
    if (!frame || frame->payload_len > TSN_MAX_PAYLOAD || frame->stream_id == 0) return TSN_EINVAL;
    return TSN_OK;
}

void tsn_frame_deliver(tsn_frame_t *frame, uint64_t rx_time_ns) {
    if (!frame) return;
    frame->rx_time_ns = rx_time_ns;
}
