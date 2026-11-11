# VitalComm

A **SIL-minded, railway-inspired message bus in Modern C++17** — built as a
personal study project to practise safety-critical embedded C++ patterns
(EN 50128 / EN 50159 style), real-time concurrent design, and a full
verification toolchain on GitLab CI.

> **Status:** Active development. See [ROADMAP.md](ROADMAP.md) for the
> milestone plan. Not production software.

## What it is

VitalComm is a small, header-first message-bus library plus a demo
application. It implements the kinds of patterns safety-critical
communication stacks rely on:

- Bounded lock-free queues (SPSC / MPSC) with **no dynamic allocation
  after initialisation**
- Static memory pools for messages and frames
- **No exceptions, no RTTI** — error handling returns `Result<T, Error>`
- Length-prefix framing over TCP with **CRC-32** integrity check
- Sequence numbers, timestamps, and **duplicate / stale frame detection**
- Configurable redundancy voting (**2oo2 / 2oo3**) across simulated
  redundant channels
- Deterministic worker threads with bounded wake-up latency

## Why

I'm building this to go deeper than the ROS 2 / robotics C++ I've shipped
in production — specifically the parts that matter in **vital software**:
determinism, static resource budgets, defensive framing, redundancy, and
a verification stack that proves it.

## Toolchain

- **Compiler:** GCC 11+ / Clang 15+, C++17
- **Build:** CMake ≥ 3.20, Ninja
- **Test:** GoogleTest, GoogleMock, libFuzzer
- **Static analysis:** clang-tidy (MISRA-adjacent ruleset), cppcheck
- **Dynamic analysis:** AddressSanitizer, ThreadSanitizer,
  UndefinedBehaviorSanitizer
- **Coverage:** gcovr / lcov
- **CI:** GitLab CI/CD (lint → build → tidy → test → sanitizers → coverage)
- **Docs:** Doxygen + an SRS-style [architecture document](docs/architecture.md)

## Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

With sanitizers (debug build):

```bash
cmake -S . -B build-asan -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DVITALCOMM_SANITIZE=address,undefined
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure
```

## Layout

```
.
├── include/vitalcomm/      public headers
├── src/                    implementation
├── tests/                  GoogleTest unit + integration
├── examples/               demo publisher / subscriber over TCP
├── docs/                   architecture.md, interface definitions
└── .gitlab-ci.yml          build / tidy / test / sanitizer / coverage
```

## Non-goals

This is a learning project. It is **not** EN 50128 certified, has not been
assessed against MISRA C++ 2008 formally, and is not safe to deploy in any
system that can injure people, damage property, or affect transport
operations.

## License

MIT. See [LICENSE](LICENSE).
