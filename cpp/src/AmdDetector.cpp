#include "AmdDetector.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace xau {

AmdDetector::AmdDetector(AmdConfig config) noexcept : config_(config) {}

const Bar& AmdDetector::newest(const std::size_t offset) const noexcept {
    const auto index = (write_index_ + BarCapacity - 1U - offset) % BarCapacity;
    return bars_[index];
}

double AmdDetector::trueRange(const std::size_t offset) const noexcept {
    const Bar& current = newest(offset);
    if (offset + 1U >= bar_count_) return current.high - current.low;
    const Bar& previous = newest(offset + 1U);
    return std::max({
        current.high - current.low,
        std::abs(current.high - previous.close),
        std::abs(current.low - previous.close)
    });
}

double AmdDetector::averageTrueRange(const std::size_t period, const std::size_t offset) const noexcept {
    if (period == 0U || bar_count_ < period + offset) return 0.0;
    double sum = 0.0;
    for (std::size_t i = offset; i < offset + period; ++i) sum += trueRange(i);
    return sum / static_cast<double>(period);
}

bool AmdDetector::detectAccumulation(Range& candidate) const noexcept {
    if (bar_count_ < config_.min_range_bars + 14U) return false;

    const double atr = averageTrueRange(14U, config_.min_range_bars);
    if (atr <= 0.0) return false;

    for (std::size_t count = config_.max_range_bars; count >= config_.min_range_bars; --count) {
        if (bar_count_ < count + 14U) {
            if (count == config_.min_range_bars) break;
            continue;
        }

        double high = -std::numeric_limits<double>::infinity();
        double low = std::numeric_limits<double>::infinity();
        bool bodies_compact = true;

        for (std::size_t i = 0; i < count; ++i) {
            const Bar& b = newest(i);
            high = std::max(high, b.high);
            low = std::min(low, b.low);
        }

        const double width = high - low;
        if (width <= 0.0 || width > atr * config_.max_range_atr_multiple) {
            if (count == config_.min_range_bars) break;
            continue;
        }

        for (std::size_t i = 0; i < count; ++i) {
            const Bar& b = newest(i);
            const double body = std::abs(b.close - b.open);
            if (body > width * config_.max_body_to_range_ratio) {
                bodies_compact = false;
                break;
            }
        }

        if (bodies_compact) {
            candidate.valid = true;
            candidate.high = high;
            candidate.low = low;
            candidate.midpoint = (high + low) * 0.5;
            candidate.width = width;
            candidate.start_timestamp_ms = newest(count - 1U).timestamp_ms;
            candidate.end_timestamp_ms = newest(0U).timestamp_ms;
            candidate.bars = static_cast<std::uint32_t>(count);
            return true;
        }

        if (count == config_.min_range_bars) break;
    }

    return false;
}

bool AmdDetector::detectManipulation(Manipulation& candidate) const noexcept {
    if (!range_.valid || bar_count_ < 15U) return false;

    const Bar& b = newest(0U);
    const double atr = averageTrueRange(14U, 1U);
    if (atr <= 0.0) return false;

    const bool swept_high = b.high > range_.high;
    const bool swept_low = b.low < range_.low;

    // A single bar sweeping both sides is ambiguous and is not classified.
    if (swept_high == swept_low) return false;

    if (swept_high) {
        const double distance = b.high - range_.high;
        if (distance < atr * config_.min_sweep_atr_multiple ||
            distance > atr * config_.max_sweep_atr_multiple) return false;
        if (config_.require_sweep_close_back_inside && b.close > range_.high) return false;

        candidate = {true, SweepSide::RangeHigh, Direction::Sell,
                     b.high, distance, b.timestamp_ms};
        return true;
    }

    const double distance = range_.low - b.low;
    if (distance < atr * config_.min_sweep_atr_multiple ||
        distance > atr * config_.max_sweep_atr_multiple) return false;
    if (config_.require_sweep_close_back_inside && b.close < range_.low) return false;

    candidate = {true, SweepSide::RangeLow, Direction::Buy,
                 b.low, distance, b.timestamp_ms};
    return true;
}

void AmdDetector::transition(const StrategyState next, AmdEvent& event) noexcept {
    if (state_ != next) {
        state_ = next;
        event.state_changed = true;
    }
    event.state = state_;
}

AmdEvent AmdDetector::onClosedM15Bar(const Bar& bar) noexcept {
    AmdEvent event{};

    bars_[write_index_] = bar;
    write_index_ = (write_index_ + 1U) % BarCapacity;
    if (bar_count_ < BarCapacity) ++bar_count_;

    if (state_ == StrategyState::Searching) {
        Range candidate{};
        if (detectAccumulation(candidate)) {
            range_ = candidate;
            transition(StrategyState::AccumulationDetected, event);
        }
    } else if (state_ == StrategyState::AccumulationDetected) {
        Manipulation candidate{};
        if (detectManipulation(candidate)) {
            manipulation_ = candidate;
            transition(StrategyState::ManipulationDetected, event);
        }
    }

    event.state = state_;
    event.range = range_;
    event.manipulation = manipulation_;
    return event;
}

void AmdDetector::reset() noexcept {
    write_index_ = 0U;
    bar_count_ = 0U;
    state_ = StrategyState::Searching;
    range_ = {};
    manipulation_ = {};
}

} // namespace xau
