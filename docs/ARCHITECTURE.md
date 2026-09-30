# Architecture

## Principle

Keep deterministic trading logic separate from research/ML.

### Python
- historical ingestion
- feature engineering
- backtests
- analytics
- walk-forward validation
- future ML experiments

### C++
- fixed-allocation tick processing
- M15/M5 market structure
- AMD state machine
- displacement and retest logic
- RSI/MACD calculations where justified
- deterministic signal generation
- structural trailing decisions
- diagnostic events

### MT5
- broker symbol discovery
- XAUUSD / XAUUSDm / XAUUSDc handling
- live prices
- contract/tick-value queries
- 1% dynamic position sizing
- session/news/spread/slippage protection
- order placement
- SL/TP updates
- position reconciliation

## Strategy lifecycle

SEARCHING
→ ACCUMULATION_DETECTED
→ MANIPULATION_DETECTED
→ AWAITING_M5_CONFIRMATION
→ DISTRIBUTION_CONFIRMED
→ AWAITING_RETEST
→ ENTRY_READY
→ TRADE_ACTIVE
→ COMPLETED / INVALIDATED

A zone invalidation is not automatically the same as invalidating the entire directional thesis. Each state transition must carry its own invalidation reason.

## Required diagnostics

Examples:

- AMD_RANGE_DETECTED
- MANIPULATION_DETECTED
- M5_BOS_CONFIRMED
- DISTRIBUTION_CONFIRMED
- ENTRY_TRIGGER=AMD_FVG_RETEST
- ENTRY_REJECTED=TOO_FAR_FROM_POI
- TRAIL_MOVED=PROTECTED_SWING
- SETUP_INVALIDATED=<reason>
- TRADE_EXITED=<reason>

Every entry family must be logged independently for later performance attribution.
