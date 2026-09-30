#include "AmdDetector.hpp"

#include <cassert>
#include <iostream>

using namespace xau;

int main() {
    AmdConfig cfg{};
    cfg.min_range_bars = 4;
    cfg.max_range_bars = 6;
    cfg.max_range_atr_multiple = 2.0;
    cfg.min_sweep_atr_multiple = 0.01;
    cfg.max_sweep_atr_multiple = 1.0;

    AmdDetector detector(cfg);

    std::int64_t t = 0;
    // Warm-up bars establish a stable ATR.
    for (int i = 0; i < 14; ++i) {
        detector.onClosedM15Bar({2000.0, 2001.0, 1999.0, 2000.2, 100.0, t += 900000});
    }

    // Compact M15 range.
    detector.onClosedM15Bar({2000.0, 2000.8, 1999.4, 2000.2, 100.0, t += 900000});
    detector.onClosedM15Bar({2000.2, 2000.9, 1999.5, 2000.1, 100.0, t += 900000});
    detector.onClosedM15Bar({2000.1, 2000.7, 1999.6, 2000.0, 100.0, t += 900000});
    const auto range_event =
        detector.onClosedM15Bar({2000.0, 2000.8, 1999.5, 2000.3, 100.0, t += 900000});

    assert(range_event.range.valid);
    assert(detector.state() == StrategyState::AccumulationDetected);

    // Sweep the range low, then close back inside: bullish manipulation.
    const auto sweep_event =
        detector.onClosedM15Bar({2000.2, 2000.5, 1999.0, 1999.8, 150.0, t += 900000});

    assert(sweep_event.manipulation.valid);
    assert(sweep_event.manipulation.side == SweepSide::RangeLow);
    assert(sweep_event.manipulation.expected_distribution == Direction::Buy);
    assert(detector.state() == StrategyState::ManipulationDetected);

    std::cout << "AMD detector smoke test passed\n";
    return 0;
}
