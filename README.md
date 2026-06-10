# CryptoBook: Real-Time Cryptocurrency Market Data and Order Book  System

## Problem Statement

Design and implement a C++ system that ingests real-time market data from a cryptocurrency exchange, constructs per-instrument order books, and exposes the resulting state for downstream consumption. The system must use Binance Spot as the initial exchange integration but be designed for extensibility to other exchanges without significant rework.

## Scope of Work

### 1. Market Data Ingestion
- Connect to Binance Spot WebSocket APIs
- Subscribe to real-time data streams:
  - Order book depth updates
  - Real-time trades
  - Tickers
- Select 10 actively traded symbols
- Persist all received market data to disk (append-only)

### 2. Order Book Construction
- Build and maintain in-memory order books per symbol
- Use received depth updates for order book construction
- Provide mechanisms to access order book state

### 3. Performance, Design, Documentation & Extensibility
System design should prioritize performance and efficiency:

**Design Considerations:**
- CPU and memory usage optimization
- Optimal data structure choices
- Threading and concurrency model
- Interaction between ingestion, processing, and persistence
- Latency implications of design decisions

---

## Solution

### Stack
**Hybrid Python + C++ Project**
- **Python:** Managed with UV (market data streaming reference implementation)
- **C++:** Managed with CMake

### Architecture Overview
**Data:**
- Binance WebSocket streams (JSON format)
- 10 largest market cap symbols (configurable):
  - BTC, ETH, XRP, BNB, SOL, TRX, DOGE, ADA, BCH, XLM
- Append-only persistence to disk

### External Dependencies
- [toml++](https://marzer.github.io/tomlplusplus/) - TOML configuration file parsing
- [ixwebsocket](https://github.com/machinezone/IXWebSocket)

### Salient Features
- OrderBook backed by a SkipList. Why? Stable references, no rebalancing, no reallocations
- Minimal heap allocations (vector)
- 1 OrderBook / Symbol / Websocket Connection per thread
- Use CRTP to trigger the correct OrderBook::onEvent function without using virtual functions
- Use Ninja for faster (incremental) builds

### How to add your own logic / algo / strategy?
- Add your logic in src/Binance.cpp. Implement the hooks, the runtime will invoke them automatically (without virtual dispatch!)

### How to expand to other exchanges?
- Instantiate include/core/OrderBook.hpp with your own types (already templated) and logic (through hooks)

### Future Scope
- Use the SBE format with zero-copy parsing?
- No vectors and everything handled through stack allocated array
- Kernel by-pass and custom hot loop instead of uWebSockets ioloop?
- Subscribe to events over shm? (to support multiple subscribers without recomiling the code)
- Core isolation and pinning (less portable code though then)

## Build and Run Instructions

### Prerequisites
- CMake 3.22+
- C++20 compatible compiler (Clang preferred)
- Python 3.10+
- UV (for Python dependency management)

### Building the C++ Project
```bash
make build
```

### Running the System
Start the market data streamer (Python):
```bash
make stream
```

Run the C++ application:
```bash
make run
```

### Code Quality
Format the code:
```bash
make format
```

Lint the code:
```bash
make lint
```

### Running Tests
```bash
make test
```

### Cleaning
```bash
make clean
```
