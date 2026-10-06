# Embedded TSN Real-Time Control

A portable C reference implementation for a distributed embedded control network using TSN-style synchronized timing, periodic traffic scheduling, bounded-latency supervision, and fault monitoring.

## What this project demonstrates

```
Control Application
        |
        v
  TSN Node Controller
        |
  +-----+------------------+
  |     |                  |
  v     v                  v
Clock Scheduler        Fault Monitor
  |     |                  |
  +-----+--------+---------+
               |
               v
       Ethernet/TSN
       Integration Layer
```

The project focuses on the embedded control logic that sits around a TSN-capable Ethernet endpoint. It deliberately keeps the network-driver boundary portable instead of pretending to implement a complete IEEE 802.1 TSN stack.

## Implemented

### Deterministic clock model
- Free-running local clock represented in nanoseconds.
- Master-to-local synchronization offset.
- Synchronized time used by the scheduler.

### Time-aware traffic scheduler
- Periodic control streams.
- Configurable period and phase.
- Per-stream latency deadline.
- Per-stream maximum scheduling jitter.
- Next-release calculation from synchronized time.

### Embedded frame model
- Fixed maximum payload of 64 bytes.
- Stream ID and sequence number.
- TX/RX timestamps.
- Payload-length validation.

### Real-time supervision
- End-to-end transport latency calculation.
- TX release-time jitter calculation.
- Deadline-miss detection.
- Jitter-violation detection.
- Minimum, maximum, average and worst-jitter statistics.
- Degraded-state reporting.

### Node-level control path
- Node initialization.
- Clock synchronization.
- Periodic control-cycle execution.
- Sequence tracking.
- Integration of scheduling and monitoring.

## Repository structure

```
Embedded-TSN-RealTime-Control/
├── .gitignore
├── Makefile
├── README.md
├── docs/
│   ├── ARCHITECTURE.md
│   ├── HARDWARE_INTEGRATION.md
│   ├── TEST_PLAN.md
│   └── TSN_MODEL.md
├── examples/
│   └── tsn_demo.c
├── include/
│   ├── tsn_clock.h
│   ├── tsn_monitor.h
│   ├── tsn_node.h
│   ├── tsn_scheduler.h
│   ├── tsn_traffic.h
│   └── tsn_types.h
├── src/
│   ├── tsn_clock.c
│   ├── tsn_monitor.c
│   ├── tsn_node.c
│   ├── tsn_scheduler.c
│   └── tsn_traffic.c
└── tests/
    └── test_tsn.c
```

## Build and run

A standard C11 compiler is sufficient.

### Run the deterministic demo

```bash
make demo
```

The example creates a periodic control stream, applies a synchronization offset, builds a control frame, records deterministic TX/RX timestamps, and reports latency and fault status.

### Run regression tests

```bash
make test
```

### Build everything

```bash
make
```

### Clean

```bash
make clean
```

## Example timing contract

The included control stream uses:

| Parameter | Value |
|---|---:|
| Stream ID | 0x10 |
| Period | 1000 us |
| Phase | 100 us |
| Deadline | 100 us |
| Max jitter | 20 us |
| Payload | 4 bytes |

This makes the timing budget explicit and easy to explain during an interview or project presentation.

## How the timing model works

For a synchronized time `t`, period `P`, and phase `phi`, the scheduler computes the nominal release as:

```
release = floor(t / P) * P + phi
```

The monitor then evaluates:

```
transport latency = RX timestamp - TX timestamp
scheduling jitter  = TX timestamp - scheduled release timestamp
```

A sample is considered healthy only when both values stay inside the stream's configured timing budgets.

## Presentation flow

A clean way to present the project is:

1. **Problem** — distributed embedded controllers need predictable communication timing.
2. **Architecture** — synchronized clock + time-aware scheduler + traffic model + timing monitor.
3. **Scheduling** — periodic stream, phase, deadline, and jitter budget.
4. **Measurement** — timestamp-based latency and release-jitter calculation.
5. **Fault handling** — deadline misses and jitter violations move the node into a degraded state.
6. **Integration** — the same APIs can be connected to a TSN-capable Ethernet MAC and hardware timestamps on a real target.

## Hardware integration boundary

The current repository is a host-runnable reference implementation. A real embedded deployment would add:
- MCU/SoC timer and synchronized timebase.
- Ethernet MAC/DMA driver.
- Hardware TX/RX timestamp capture.
- TSN traffic-class / gate configuration.
- An IEEE 802.1AS/gPTP implementation.
- Interrupt, watchdog, and diagnostics integration.
- Board-specific build and linker configuration.

See [docs/HARDWARE_INTEGRATION.md](docs/HARDWARE_INTEGRATION.md).

## Verification scope

The software tests validate the algorithms and data structures, including synchronization, scheduling, frame validation, latency/jitter classification, fault detection, and the node control path.

They do **not** claim physical TSN-network validation, real switch-queue measurements, hardware timestamp accuracy, IEEE 802.1AS interoperability, or FPGA/MCU timing closure.

See:
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)
- [docs/TSN_MODEL.md](docs/TSN_MODEL.md)
- [docs/TEST_PLAN.md](docs/TEST_PLAN.md)

## Skills demonstrated

**Embedded C · Real-Time Systems · Ethernet/TSN Concepts · Scheduling · Time Synchronization · Timestamp-Based Diagnostics · Fault Monitoring · Modular Driver Architecture · Unit Testing · Systems Documentation**

## License

MIT License.
