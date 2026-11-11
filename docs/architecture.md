# VitalComm — Architecture &amp; Subsystem Specification

> SRS-style working document. Edited as the design evolves; each change
> is paired with the commit that implements it.

## 1. Purpose and scope

VitalComm is a small C++17 message-bus library that applies patterns
common in safety-critical railway communication stacks:

- Bounded, static resource allocation
- Deterministic worker threads
- End-to-end frame integrity via CRC-32
- Replay / staleness protection via monotonic sequence numbers
- Optional voting across redundant channels

The project is a personal learning study, not a certified component.
Specifications below use **shall** / **should** / **may** per RFC 2119.

## 2. System overview

```
┌──────────────┐    frame(seq, ts, payload, crc32)    ┌──────────────┐
│  Publisher   │  ───────────────────────────────────▶ │  Subscriber  │
│   thread     │   via TCP transport (channel A)      │   thread     │
└──────┬───────┘                                       └──────▲───────┘
       │                                                      │
       │        ┌────────────┐                                │
       └──────▶ │  Voter N/K │  ◀──── channel B / channel C ──┘
                └────────────┘
```

## 3. Subsystems

### 3.1 Core utilities (`vitalcomm::core`)

- **`Result<T, Error>`** — exception-free error carrier.
- **`StaticBuffer<N>`** — fixed-capacity buffer, bounds-checked.
- **`crc32`** — RFC 1952 / Ethernet CRC-32; constant-time table lookup.

### 3.2 Queueing (`vitalcomm::queue`)

- `SpscRing<T, N>` — single-producer, single-consumer lock-free ring.
- `MpscQueue<T, N>` — multi-producer, single-consumer bounded queue.
- Both **shall** operate without dynamic allocation after construction.

### 3.3 Framing (`vitalcomm::frame`)

A frame on the wire is:

| Field       | Size      | Notes                                    |
|-------------|-----------|------------------------------------------|
| length      | 2 bytes   | big-endian, payload length (≤ 1024)      |
| sequence    | 4 bytes   | big-endian, monotonic per publisher      |
| timestamp   | 8 bytes   | big-endian, monotonic ns since boot      |
| payload     | N bytes   | opaque to the transport                  |
| crc32       | 4 bytes   | over length..payload, big-endian         |

Decoding **shall** reject: wrong length, bad CRC, out-of-window sequence
numbers, stale timestamps.

### 3.4 Transport (`vitalcomm::net`)

- POSIX TCP; non-blocking sockets; one `Listener` + many `Connection`.
- Worker threads **should** be pinned to a specific core.
- Back-pressure policy **may** be configured as drop-oldest or block.

### 3.5 Voting (`vitalcomm::vote`)

- `Voter<N, K>` with compile-time `N` channels and `K`-of-`N` majority.
- A result is emitted only when `K` channels agree on `(seq, payload)`
  within a configurable time window.

## 4. Non-functional requirements

| ID    | Requirement                                                       |
|-------|-------------------------------------------------------------------|
| NF-1  | No dynamic allocation **after** `init()` in the queueing / framing layers. |
| NF-2  | No exceptions, no RTTI in the library.                            |
| NF-3  | Clean under ASan, UBSan, TSan on CI.                              |
| NF-4  | ≥ 80 % line coverage over `include/` and `src/`.                  |
| NF-5  | Deterministic wake-up latency: p99 ≤ 500 µs on a quiet machine.   |
| NF-6  | Buildable with GCC 11 and Clang 15 on Ubuntu 22.04.               |

## 5. Verification

| Layer           | Technique                             |
|-----------------|---------------------------------------|
| Core utilities  | Unit tests + known-answer vectors     |
| Queues          | Unit + stress under TSan              |
| Framing         | Unit + libFuzzer on decode path       |
| Transport       | Integration tests with loopback       |
| Voter           | Property tests with fault injection   |

## 6. Open items

- Choose between `std::jthread` and raw `pthread_create` for worker
  pinning. Trade-off: portability vs explicit CPU-set control.
- Decide the back-pressure default. Drop-oldest is simpler; blocking
  matches control-loop semantics more closely.

## 7. References

- EN 50128 — Railway applications — Software for railway control and
  protection systems.
- EN 50159 — Railway applications — Safety-related communication in
  transmission systems.
- RFC 1952 — GZIP file format specification (CRC-32).
- Barr Group — Embedded C Coding Standard.
