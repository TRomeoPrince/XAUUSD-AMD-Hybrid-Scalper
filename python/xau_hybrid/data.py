from __future__ import annotations

import pandas as pd


REQUIRED_COLUMNS = ("time", "open", "high", "low", "close", "volume")


def validate_ohlcv(frame: pd.DataFrame) -> pd.DataFrame:
    """Validate and normalize OHLCV input used by research/backtests."""

    missing = [column for column in REQUIRED_COLUMNS if column not in frame.columns]
    if missing:
        raise ValueError(f"Missing OHLCV columns: {missing}")

    data = frame.loc[:, REQUIRED_COLUMNS].copy()
    data["time"] = pd.to_datetime(data["time"], utc=True)

    numeric = ["open", "high", "low", "close", "volume"]
    data[numeric] = data[numeric].apply(pd.to_numeric, errors="raise")

    if (data[numeric].isna().any().any()):
        raise ValueError("OHLCV data contains NaN values")

    if (not (data["high"] >= data[["open", "close", "low"]].max(axis=1)).all()
            or not (data["low"] <= data[["open", "close", "high"]].min(axis=1)).all()):
        raise ValueError("Invalid OHLC bar geometry")

    return data.sort_values("time").reset_index(drop=True)
