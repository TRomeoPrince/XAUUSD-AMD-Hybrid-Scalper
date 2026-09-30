# XAUUSD AMD Hybrid Scalper

Hybrid research and execution architecture for an XAUUSD scalping system.

## Architecture

- **Python**: historical data ingestion, research, backtesting, feature engineering, analytics, and future ML experiments.
- **C++**: real-time M15/M5 strategy engine, market-structure calculations, AMD state machine, indicators, signal generation, and diagnostic logging.
- **MT5 / MQL5**: broker integration, live symbol handling, 1% risk sizing, execution, SL/TP, session/news/spread protection, and position management.

## Current Strategy Identity

The system is designed around **M15 context + M5 execution**.

### M15
- Market structure
- Supply/demand and important support/resistance areas
- Accumulation/range detection
- Liquidity around the range
- Manipulation / liquidity sweep
- Expected distribution direction

### M5
- Liquidity refinement
- BOS / CHoCH / MSS confirmation
- Momentum / displacement
- FVG / order-block / structural retests where appropriate
- Candlestick confirmation
- Entry execution

Indicators are secondary:
- **RSI**: mainly reversal confirmation
- **MACD**: mainly continuation / momentum confirmation

AMD and price action remain the primary decision framework.

## Version

Current foundation: **v0.1.0**

This version establishes:
- project layout
- shared market-data types
- fixed-size C++ tick buffer
- initial market-structure scaffolding
- diagnostic event logging
- Python package skeleton
- MT5 bridge skeleton

No live auto-trading is enabled in v0.1.0.

## Planned Development

1. v0.1.x — hybrid infrastructure and diagnostics
2. v0.2.0 — M15 accumulation + AMD state machine
3. v0.3.0 — M5 BOS/MSS/displacement + entry families
4. v0.4.0 — execution/risk/session/news protections
5. v0.5.0 — historical analytics and strategy comparison
6. Later — optional ML confidence/filter layer after deterministic edge is validated

## Risk

Planned production risk:
- 1% of account equity per trade
- dynamic lot sizing from actual SL and broker contract specifications
- support for XAUUSD symbol variants such as XAUUSD, XAUUSDm, and XAUUSDc

## Status

Research / development only. Do not use on a live account until the strategy has been validated through historical, walk-forward, and forward-demo testing.
