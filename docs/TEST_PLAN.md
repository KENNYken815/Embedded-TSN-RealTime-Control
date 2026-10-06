# Test Plan

## Host tests

Run:

```bash
make test
```

The tests cover:
- clock progression and synchronization
- periodic release calculation
- frame build/validation and payload limits
- latency and jitter classification
- min/max/average latency accounting
- deadline and jitter fault detection
- complete node control-cycle execution

## Demo

Run:

```bash
make demo
```

The demo uses deterministic timestamps so the timing scenario is reproducible without a physical network.

## Verification boundary

Host tests validate software algorithms and data structures. They do not prove:
- wire-rate Ethernet performance
- real switch queue behavior
- IEEE 802.1AS interoperability
- hardware timestamp accuracy
- FPGA/MCU timing closure
- end-to-end physical-network latency

Those require a concrete hardware and network setup.
