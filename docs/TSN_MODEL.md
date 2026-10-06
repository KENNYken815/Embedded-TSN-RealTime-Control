# TSN Timing Model

## Stream contract

Each stream contains:
- period in microseconds
- phase in microseconds
- latency deadline in microseconds
- maximum scheduling jitter in microseconds

The next release is calculated from synchronized time:

```
release = floor(t / period) * period + phase
```

If that release has already passed, the scheduler advances to the next period.

## Latency

```
latency = rx_timestamp - tx_timestamp
```

The result is checked against the stream deadline.

## Jitter

```
jitter = tx_start_timestamp - scheduled_release_timestamp
```

This measures release-time determinism separately from transport latency.

## Engineering value

The model makes timing budgets visible and testable before mapping the design onto a concrete TSN endpoint. It demonstrates periodic control traffic, deterministic scheduling, bounded-latency supervision, and degraded-state detection.
