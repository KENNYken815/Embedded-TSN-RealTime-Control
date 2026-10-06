# Architecture

## System flow

```
Distributed control application
        |
        v
TSN node state machine
        |
        +--> synchronized clock
        +--> time-aware scheduler
        +--> deterministic traffic model
        +--> latency/jitter monitor
        +--> fault status
        |
        v
Ethernet/TSN MAC + PHY integration boundary
```

| Component | Responsibility |
|---|---|
| Clock | Maintains local time and applies synchronization offset |
| Scheduler | Defines periodic streams, phase, deadline, and jitter budget |
| Traffic | Builds and validates bounded-size deterministic frames |
| Monitor | Measures latency/jitter and counts timing violations |
| Node | Coordinates clock, schedule, and monitoring |
| Example | Runs a deterministic distributed-control scenario |

The repository models TSN control behavior without claiming to implement a complete IEEE 802.1 networking stack. A real target connects these APIs to TSN-capable MAC hardware, hardware timestamping, switch configuration, and a synchronization implementation such as gPTP/802.1AS.
