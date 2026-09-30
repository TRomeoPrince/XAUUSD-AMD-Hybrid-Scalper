#include "TradingEngine.hpp"

namespace xau {

TradingEngine::TradingEngine(AmdConfig amd_config) noexcept
    : amd_detector_(amd_config) {}

void TradingEngine::onTick(const Tick& tick) noexcept {
    ticks_[write_index_] = tick;
    write_index_ = (write_index_ + 1U) % TickBufferSize;

    if (tick_count_ < TickBufferSize) {
        ++tick_count_;
    }
}

AmdEvent TradingEngine::onClosedM15Bar(const Bar& bar) noexcept {
    return amd_detector_.onClosedM15Bar(bar);
}

std::size_t TradingEngine::tickCount() const noexcept {
    return tick_count_;
}

StrategyState TradingEngine::state() const noexcept {
    return amd_detector_.state();
}

const Range& TradingEngine::activeM15Range() const noexcept {
    return amd_detector_.activeRange();
}

const Manipulation& TradingEngine::activeManipulation() const noexcept {
    return amd_detector_.manipulation();
}

const Tick& TradingEngine::tickFromNewest(const std::size_t offset) const noexcept {
    const auto index =
        (write_index_ + TickBufferSize - 1U - offset) % TickBufferSize;

    return ticks_[index];
}

Signal TradingEngine::evaluate() noexcept {
    Signal signal{};

    if (tick_count_ == 0U) {
        return signal;
    }

    const Tick& current = tickFromNewest(0U);
    signal.entry_price = (current.bid + current.ask) * 0.5;

    if (amd_detector_.manipulation().valid) {
        signal.direction = amd_detector_.manipulation().expected_distribution;
    }

    // Safety by design: v0.2.0 exposes context but never triggers a trade.
    // M5 confirmation must be implemented before trigger_trade can become true.
    signal.trigger_trade = false;
    return signal;
}

} // namespace xau
