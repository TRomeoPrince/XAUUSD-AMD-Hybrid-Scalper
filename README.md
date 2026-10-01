# XAUUSD AMD Hybrid Scalper

Hybrid M15/M5 XAUUSD scalping research and execution system.

## Architecture

- **Python** — historical ingestion, backtests, analytics and future ML research.
- **C++** — deterministic AMD, market-structure and setup engine.
- **MT5/MQL5** — future broker integration, risk/execution and safety controls.

## Strategy

**M15:** accumulation → liquidity/manipulation → expected distribution direction.

**M5:** confirmed swing structure → body-close BOS/MSS evidence → displacement → protected swing → retest → setup candidate.

Indicators remain secondary confirmations. AMD and price action remain primary.

## Current version — v0.3.0

Implemented:
- configurable M15 accumulation detector
- M15 one-sided liquidity sweep/manipulation
- expected distribution direction
- M5 confirmed swing detection
- body-close structure break
- ATR/body-ratio displacement confirmation
- protected swing capture
- AMD structural-retest candidate
- exact `ENTRY_TRIGGER=AMD_STRUCTURE_RETEST`
- explicit setup rejection reasons
- smoke tests for AMD and M5 confirmation
- TradingEngine integration across M15 → M5

### Safety lock

v0.3.0 **does not send trades**. Even a valid setup candidate leaves `Signal::trigger_trade=false`. Broker execution is intentionally deferred until setup detection has been replayed and validated.

## Next

v0.3.x / v0.4 work:
- FVG detection and retest
- order-block/rejection entry families
- refined BOS vs CHoCH/MSS classification
- setup expiry and POI-distance rules
- diagnostic log serialization for Python analysis
- session/news/spread filters
- broker-aware 1% risk engine
- MT5 ↔ C++ live bridge

The current numerical thresholds are research defaults and must be tuned using historical XAUUSD data rather than treated as final strategy truth.
