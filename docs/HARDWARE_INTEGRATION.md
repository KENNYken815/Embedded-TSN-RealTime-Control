# Hardware Integration

This is a portable reference implementation. Vendor- and board-specific Ethernet behavior is intentionally kept at the integration boundary.

## Target mapping

1. Map the software clock to a free-running hardware timer and synchronized time base.
2. Feed hardware TX/RX timestamps into the latency measurement API.
3. Connect scheduler release decisions to a TSN-capable MAC transmit queue or hardware gate.
4. Configure stream traffic classes and network admission in the target switch.
5. Map synchronization to a selected IEEE 802.1AS/gPTP implementation.
6. Route timing faults to the target diagnostics/watchdog path.

## Example stack

```
Control application
       |
TSN node API
       |
RTOS / bare-metal scheduler
       |
TSN-capable Ethernet MAC
       |
PHY
       |
TSN switch fabric
```

A target integration normally supplies:
- timer/clock driver
- TX/RX timestamp capture
- Ethernet DMA descriptors
- interrupts
- traffic-class/gate configuration
- synchronization protocol
- diagnostics/watchdog integration

No particular MCU, RTOS, Ethernet controller, switch, or commercial TSN stack is claimed here.
