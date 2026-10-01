#include "TradingEngine.hpp"

namespace xau {

TradingEngine::TradingEngine(AmdConfig amd_config, M5Config m5_config) noexcept
    : amd_detector_(amd_config), m5_engine_(m5_config) {}

void TradingEngine::onTick(const Tick& tick) noexcept {
    ticks_[write_index_] = tick;
    write_index_ = (write_index_ + 1U) % TickBufferSize;
    if (tick_count_ < TickBufferSize) ++tick_count_;
}

AmdEvent TradingEngine::onClosedM15Bar(const Bar& bar) noexcept {
    return amd_detector_.onClosedM15Bar(bar);
}

SetupCandidate TradingEngine::onClosedM5Bar(const Bar& bar) noexcept {
    const auto& manipulation = amd_detector_.manipulation();
    const Direction expected = manipulation.valid
        ? manipulation.expected_distribution
        : Direction::None;

    const M5Confirmation next = m5_engine_.onClosedBar(bar, expected);
    if (next.valid) m5_confirmation_ = next;

    if (!m5_confirmation_.valid) {
        latest_setup_ = {};
        latest_setup_.rejection = manipulation.valid
            ? RejectionReason::NoM5Confirmation
            : RejectionReason::NoM15Manipulation;
        return latest_setup_;
    }

    const double max_distance = m5_confirmation_.atr * 0.20;
    latest_setup_ = setup_engine_.evaluateRetest(
        bar, manipulation, m5_confirmation_, max_distance);
    return latest_setup_;
}

std::size_t TradingEngine::tickCount() const noexcept { return tick_count_; }
StrategyState TradingEngine::state() const noexcept { return amd_detector_.state(); }
const Range& TradingEngine::activeM15Range() const noexcept { return amd_detector_.activeRange(); }
const Manipulation& TradingEngine::activeManipulation() const noexcept { return amd_detector_.manipulation(); }
const M5Confirmation& TradingEngine::m5Confirmation() const noexcept { return m5_confirmation_; }
const SetupCandidate& TradingEngine::latestSetup() const noexcept { return latest_setup_; }

const Tick& TradingEngine::tickFromNewest(const std::size_t offset) const noexcept {
    const auto index = (write_index_ + TickBufferSize - 1U - offset) % TickBufferSize;
    return ticks_[index];
}

Signal TradingEngine::evaluate() noexcept {
    Signal signal{};
    if (tick_count_ == 0U) return signal;

    const Tick& current = tickFromNewest(0U);
    signal.entry_price = (current.bid + current.ask) * 0.5;

    if (latest_setup_.ready) {
        signal.direction = latest_setup_.direction;
        signal.entry_trigger = latest_setup_.trigger;
        signal.stop_loss = latest_setup_.stop;
        signal.entry_price = latest_setup_.entry;
    } else if (amd_detector_.manipulation().valid) {
        signal.direction = amd_detector_.manipulation().expected_distribution;
    }

    // v0.3.0 can form setup candidates but broker execution remains locked.
    signal.trigger_trade = false;
    return signal;
}

} // namespace xau
