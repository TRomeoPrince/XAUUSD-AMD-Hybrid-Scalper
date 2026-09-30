#include "TradingEngine.hpp"

namespace xau {

void TradingEngine::onTick(const Tick& tick) noexcept {
    ticks_[write_index_] = tick;
    write_index_ = (write_index_ + 1U) % TickBufferSize;

    if (tick_count_ < TickBufferSize) {
        ++tick_count_;
    }
}

std::size_t TradingEngine::tickCount() const noexcept {
    return tick_count_;
}

StrategyState TradingEngine::state() const noexcept {
    return state_;
}

const Tick& TradingEngine::tickFromNewest(const std::size_t offset) const noexcept {
    const auto index =
        (write_index_ + TickBufferSize - 1U - offset) % TickBufferSize;

    return ticks_[index];
}

void TradingEngine::setState(const StrategyState next_state) noexcept {
    state_ = next_state;
}

Signal TradingEngine::evaluate() noexcept {
    Signal signal{};

    if (tick_count_ == 0U) {
        return signal;
    }

    const Tick& current = tickFromNewest(0U);
    signal.entry_price = (current.bid + current.ask) * 0.5;

    // v0.1.0 deliberately does not create live trading signals.
    // The M15 AMD state machine and M5 execution logic are added in v0.2+.
    return signal;
}

} // namespace xau
