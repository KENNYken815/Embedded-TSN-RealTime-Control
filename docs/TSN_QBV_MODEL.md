# Qbv-Style Time-Aware Gate Model

This repository includes a cyclic time-aware gate schedule inspired by IEEE 802.1Qbv.

Each gate-control-list entry defines an 8-bit gate mask and an interval duration in microseconds. The schedule repeats after the sum of all intervals.

Example:

| Interval | Gate mask | Traffic class |
|---:|---:|---|
| 100 us | 0x01 | control |
| 200 us | 0x02 | telemetry |
| 100 us | 0x04 | diagnostics |

This is a deterministic software model, not an Ethernet switch implementation or IEEE conformance test. Real deployment requires a TSN-capable MAC/switch, 802.1AS/gPTP synchronization, hardware timestamping, and platform-specific configuration.
