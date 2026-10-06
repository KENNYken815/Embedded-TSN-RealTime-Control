# Failure Handling

The monitor tracks dropped frames, deadline misses, jitter violations, and clock synchronization faults.

The node is considered degraded when deadline or jitter violations are observed.

A hardware deployment should define a safe response such as:
1. reject stale control frames;
2. hold or transition actuators to a defined safe state;
3. raise a diagnostic event;
4. attempt network/clock recovery;
5. require application-level recovery before returning to normal control.

This repository models the monitoring decision only. The actuator safe state belongs to the target application's safety architecture.
