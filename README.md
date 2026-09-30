# XAUUSD AMD Hybrid Scalper

Hybrid M15/M5 XAUUSD scalping research and execution system.

## Architecture

- **Python** — historical ingestion, research, backtests, analytics and future ML experiments.
- **C++** — deterministic real-time strategy engine and diagnostics.
- **MT5/MQL5** — broker integration, risk/execution and safety controls.

## Strategy identity

**M15 = context / AMD.** Market structure, accumulation, liquidity, manipulation and distribution narrative.

**M5 = execution.** BOS/MSS, displacement, FVG/OB/structural retests and price-action confirmation.

RSI and MACD remain secondary confirmation tools; they do not create trades independently.

## Current version — v0.2.0

Implemented:
- fixed-size tick and M15 bar processing
- configurable M15 accumulation detector
- range high/low/midpoint/width metadata
- one-sided liquidity-sweep/manipulation detector
- provisional expected distribution direction
- explicit AMD states
- smoke test for range → low sweep → bullish expectation
- documentation of detector assumptions

Not implemented yet:
- M5 BOS/MSS/displacement confirmation
- FVG/OB/retest entry families
- live trade signals
- 1% broker-aware execution
- news/session/spread protection
- ML decision filter

## Research warning

The v0.2 accumulation thresholds are configurable starting hypotheses. They must be validated against historical XAUUSD examples and the intended visual definition before being promoted to production strategy rules.

## Planned risk

Production target remains 1% equity risk per trade with broker-aware dynamic sizing and XAUUSD/XAUUSDm/XAUUSDc compatibility.

Do not use this repository for live trading until historical, walk-forward and forward-demo validation is complete.
