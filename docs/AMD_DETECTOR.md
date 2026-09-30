# M15 AMD Detector — v0.2.0

## Purpose

v0.2.0 introduces the first deterministic M15 AMD state machine. It detects a candidate accumulation range and then watches for a one-sided liquidity sweep.

It intentionally does **not** generate trades. M5 confirmation is a later stage.

## Accumulation candidate

A candidate range currently requires:

- configurable 4–12 closed M15 bars
- total high-to-low range constrained relative to prior ATR(14)
- candle bodies constrained relative to the total range width
- fixed-size buffers; no allocation in the bar-processing path

These are research defaults, not a claim that they are the final discretionary definition.

## Manipulation candidate

After a range is locked:

- price must sweep exactly one range boundary
- sweep distance must be within configurable ATR bounds
- by default the sweep candle must close back inside the range
- high sweep implies provisional bearish distribution expectation
- low sweep implies provisional bullish distribution expectation
- a candle sweeping both sides is treated as ambiguous

## Important distinction

A wick sweep and a body-close breakout are intentionally different events.

A sweep may be manipulation. A decisive body close beyond a range may instead be the start of breakout/distribution logic. v0.3 will classify that path rather than incorrectly treating every excursion as manipulation.

## Next validation work

Before live entry logic:
1. replay historical M15 bars
2. log every detected range
3. visually compare detections with the user's intended accumulation examples
4. tune or replace range rules based on evidence
5. then add M5 BOS/MSS/displacement confirmation
