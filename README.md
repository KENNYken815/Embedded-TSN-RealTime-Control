# Embedded TSN Real-Time Control

A host-runnable embedded C reference implementation for deterministic Ethernet control concepts inspired by Time-Sensitive Networking (TSN).

The project models synchronized distributed nodes, periodic control-stream scheduling, Qbv-style time-aware gates, latency/jitter monitoring, and degraded-mode fault detection.

> **Scope:** This is an educational/reference software model. It does not claim IEEE TSN conformance, physical Ethernet timing measurements, FPGA/MCU validation, or production safety certification.

## Architecture

    Distributed Node
           |
    Synchronized Clock
           |
    Periodic Scheduler
           |
    Qbv-Style Gate Control
           |
    Timestamped Control Frame
           |
    Latency/Jitter Monitor
           |
    Fault / Degraded State

## Implemented Features

### 1. Synchronized distributed clock model
- Node-local nanosecond clock.
- Master-to-local synchronization offset.
- Synchronized time calculation.
- Explicit synchronization state.

### 2. Deterministic traffic scheduling
Each stream can define:
- stream ID;
- traffic class;
- period;
- phase;
- deadline;
- maximum release jitter.

The scheduler calculates the next deterministic release point from synchronized time.

### 3. Qbv-style time-aware gate control
tsn_gcl.c implements a cyclic gate-control list inspired by IEEE 802.1Qbv.

Example schedule:

| Interval | Gate mask | Purpose |
|---:|---:|---|
| 100 us | 0x01 | control |
| 200 us | 0x02 | telemetry |
| 100 us | 0x04 | diagnostics |

The gate model determines which traffic class is permitted at a given synchronized time.

### 4. Timestamped frame model
Frames contain:
- stream ID;
- sequence number;
- transmit timestamp;
- receive timestamp;
- bounded payload.

Input validation prevents payload overflow.

### 5. Real-time performance monitoring
For every measured sample the monitor tracks:
- latency;
- release jitter;
- minimum latency;
- maximum latency;
- average latency;
- deadline misses;
- jitter violations.

### 6. Fault/degraded-state detection
The monitoring layer flags the control network as degraded when timing requirements are violated.

The project documents a safe-response concept for a real embedded deployment, while leaving actuator safety behavior to the target system.

## Repository Structure

    Embedded-TSN-RealTime-Control/
    +-- .github/workflows/ci.yml
    +-- docs/
    |   +-- ARCHITECTURE.md
    |   +-- FAILURE_HANDLING.md
    |   +-- HARDWARE_INTEGRATION.md
    |   +-- TEST_PLAN.md
    |   +-- TSN_MODEL.md
    |   +-- TSN_QBV_MODEL.md
    +-- examples/
    |   +-- tsn_demo.c
    +-- include/
    |   +-- tsn_clock.h
    |   +-- tsn_gcl.h
    |   +-- tsn_monitor.h
    |   +-- tsn_node.h
    |   +-- tsn_scheduler.h
    |   +-- tsn_traffic.h
    |   +-- tsn_types.h
    +-- src/
    |   +-- tsn_clock.c
    |   +-- tsn_gcl.c
    |   +-- tsn_monitor.c
    |   +-- tsn_node.c
    |   +-- tsn_scheduler.c
    |   +-- tsn_traffic.c
    +-- tests/
    |   +-- test_gcl.c
    |   +-- test_tsn.c
    +-- .gitignore
    +-- Makefile
    +-- README.md

## Build and Run

Requirements:
- C11 compiler such as GCC or Clang;
- GNU Make.

Build everything:

    make

Run the real-time control demonstration:

    make demo

Run core tests:

    make test

Run Qbv-style gate-control tests:

    make gcl-test

Clean generated files:

    make clean

GitHub Actions also builds and runs the project test targets on pushes and pull requests.

## What the Demo Demonstrates

The example creates a distributed control node, applies a synchronization offset, configures a 1 ms control stream, creates a timestamped frame, measures simulated transport latency, and reports timing health.

This makes the repository useful for demonstrating the embedded reasoning behind deterministic networking without pretending that a desktop simulation is equivalent to physical TSN timing validation.

## Engineering Concepts Covered

- embedded C;
- deterministic scheduling;
- periodic real-time tasks;
- clock synchronization concepts;
- timestamp-based latency measurement;
- jitter budgets;
- deadline monitoring;
- cyclic gate-control lists;
- distributed control nodes;
- fault/degraded-state handling;
- modular driver-style interfaces;
- unit testing;
- build automation and CI.

## Hardware Integration Path

A physical implementation would replace the software time/frame model with:
1. MCU/SoC timer hardware;
2. Ethernet MAC with timestamp support;
3. TSN-capable switch or endpoint;
4. IEEE 802.1AS/gPTP clock synchronization;
5. hardware time-aware shaping/Qbv configuration;
6. DMA/descriptor management;
7. interrupt-driven packet handling;
8. measured oscilloscope/packet-timestamp latency validation.

The current repository deliberately keeps those platform-specific layers separate.

## Interview / Presentation Flow

1. **Problem:** conventional Ethernet does not by itself guarantee deterministic control timing.
2. **Clock:** distributed nodes need a common time reference.
3. **Schedule:** periodic streams receive explicit period, phase, deadline, and jitter budgets.
4. **Gate:** Qbv-style cyclic gates reserve transmission windows for traffic classes.
5. **Measure:** timestamps produce latency and jitter samples.
6. **Detect:** timing violations move the node into a degraded state.
7. **Deploy:** replace the reference model with a TSN-capable Ethernet platform and validate timing in hardware.

## Verification Scope

The repository contains deterministic software tests for:
- clock synchronization;
- schedule lookup and release calculation;
- frame construction and validation;
- latency/deadline/jitter evaluation;
- node control-cycle behavior;
- Qbv-style gate selection.

The CI workflow is configured to run the build and test suite automatically.

No claim is made that these tests prove IEEE TSN conformance or real-world network determinism.

## Future Extensions

Possible next steps:
- IEEE 802.1AS/gPTP message/state-machine model;
- ingress/egress hardware timestamp abstraction;
- more complete 802.1Qbv schedule configuration;
- frame-preemption model;
- UDP/Ethernet socket integration;
- packet capture analysis;
- Linux TAP/SocketCAN-style test harness;
- FPGA/MCU target port;
- hardware-in-the-loop latency measurements.

## License

MIT License.
