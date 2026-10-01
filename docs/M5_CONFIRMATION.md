# M5 Confirmation Layer — v0.3.0

v0.3.0 adds deterministic M5 confirmation after an M15 manipulation has established an expected distribution direction.

## Confirmation sequence

1. M15 manipulation provides expected direction.
2. M5 searches confirmed local swings.
3. A candle must **close beyond** the relevant swing level.
4. The same break candle must qualify as displacement using configurable ATR/body rules.
5. The opposite local swing is recorded as the protected swing.
6. A later M5 bar may retest the broken structure.
7. If the level is touched and held, the candidate is tagged:
   - `ENTRY_TRIGGER=AMD_STRUCTURE_RETEST`

This produces a **setup candidate**, not a broker order.

## Why body close?

A wick through structure is not treated as a BOS. This prevents the engine from confusing a liquidity probe with a confirmed structural break.

## Protected swing

For bullish confirmation the most recent confirmed swing low is retained. For bearish confirmation the most recent confirmed swing high is retained. This becomes the structural SL reference and later trailing reference.

## Rejection reasons

Candidates can currently be rejected as:
- NO_M15_MANIPULATION
- NO_M5_CONFIRMATION
- WRONG_DIRECTION
- RETEST_NOT_REACHED
- TOO_FAR_FROM_POI

These categories are intended for backtest attribution.

## Still pending

- FVG detection/retest
- order-block detection/retest
- rejection-candle entry family
- breakout/retest family outside AMD
- refined CHoCH vs BOS classification
- session/news/spread filters
- broker execution
