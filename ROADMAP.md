# Roadmap

Milestone-based plan so commits track a real trajectory. Each milestone
closes with a tagged release and a green CI pipeline.

## M1 — Foundations  *(week 1)*

- [x] Project scaffolding (CMake, Ninja, gtest fetched via `FetchContent`)
- [ ] `Result<T, Error>` type (no exceptions, no RTTI)
- [ ] Fixed-capacity `StaticBuffer<N>` with bounds checks
- [ ] `CRC32` computation + unit tests against RFC 1952 vectors
- [ ] GitLab CI: build + unit test matrix (gcc / clang, Release / Debug)
- [ ] clang-format + clang-tidy baseline passing

## M2 — Queueing primitives  *(week 1–2)*

- [ ] Bounded **SPSC ring buffer** (lock-free, cache-line padded)
- [ ] Bounded **MPSC queue** with `std::atomic` sequence tickets
- [ ] Throughput + latency micro-benchmarks
- [ ] ThreadSanitizer-clean under stress tests
- [ ] libFuzzer harness for the queue API

## M3 — Framing &amp; integrity  *(week 2)*

- [ ] Length-prefix `Frame<N>` encoder / decoder
- [ ] CRC-32 appended + verified on decode
- [ ] Monotonic `sequence_t` and `timestamp_t`
- [ ] Duplicate / stale detection window
- [ ] Fuzzing of the decoder against random / malformed input

## M4 — Transport  *(week 2–3)*

- [ ] POSIX TCP `Listener` / `Connection` (non-blocking, `epoll`-ready)
- [ ] Deterministic worker threads (pinned, bounded wake-up)
- [ ] Back-pressure policy (drop-oldest vs block, configurable)
- [ ] Integration test: pub / sub echo with injected packet loss

## M5 — Redundancy &amp; voting  *(week 3)*

- [ ] `Voter<N, K>` compile-time configurable 2oo2 / 2oo3
- [ ] Channel abstraction over two independent transports
- [ ] Fault-injection tests (channel drop, bit flip, reorder)

## M6 — Verification &amp; docs  *(week 3–4)*

- [ ] Coverage target: ≥ 80% line, ≥ 70% branch
- [ ] Sanitizer jobs green on CI (ASan, TSan, UBSan)
- [ ] `docs/architecture.md` complete (SRS-style)
- [ ] Doxygen generation in CI
- [ ] `v0.1.0` tag + release notes

## Stretch

- MISRA C++ 2008 self-audit against a chosen rule subset
- Formal CRC-32 property-based tests
- `std::pmr` memory pools behind a feature flag
