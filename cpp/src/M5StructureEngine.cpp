#include "M5StructureEngine.hpp"

#include <algorithm>
#include <cmath>

namespace xau {

M5StructureEngine::M5StructureEngine(M5Config config) noexcept : config_(config) {}

const Bar& M5StructureEngine::newest(const std::size_t offset) const noexcept {
    const auto index = (write_index_ + BarCapacity - 1U - offset) % BarCapacity;
    return bars_[index];
}

double M5StructureEngine::trueRange(const std::size_t offset) const noexcept {
    const Bar& current = newest(offset);
    if (offset + 1U >= bar_count_) return current.high - current.low;
    const Bar& previous = newest(offset + 1U);
    return std::max({
        current.high - current.low,
        std::abs(current.high - previous.close),
        std::abs(current.low - previous.close)
    });
}

double M5StructureEngine::atr(const std::size_t period, const std::size_t offset) const noexcept {
    if (period == 0U || bar_count_ < period + offset) return 0.0;
    double sum = 0.0;
    for (std::size_t i = offset; i < offset + period; ++i) sum += trueRange(i);
    return sum / static_cast<double>(period);
}

bool M5StructureEngine::isSwingHigh(const std::size_t offset) const noexcept {
    if (offset < config_.swing_right ||
        offset + config_.swing_left >= bar_count_) return false;

    const double p = newest(offset).high;
    for (std::size_t i = 1; i <= config_.swing_left; ++i)
        if (newest(offset + i).high >= p) return false;
    for (std::size_t i = 1; i <= config_.swing_right; ++i)
        if (newest(offset - i).high > p) return false;
    return true;
}

bool M5StructureEngine::isSwingLow(const std::size_t offset) const noexcept {
    if (offset < config_.swing_right ||
        offset + config_.swing_left >= bar_count_) return false;

    const double p = newest(offset).low;
    for (std::size_t i = 1; i <= config_.swing_left; ++i)
        if (newest(offset + i).low <= p) return false;
    for (std::size_t i = 1; i <= config_.swing_right; ++i)
        if (newest(offset - i).low < p) return false;
    return true;
}

bool M5StructureEngine::latestSwingHigh(double& price) const noexcept {
    const std::size_t start = config_.swing_right + 1U; // exclude current break bar
    for (std::size_t i = start; i + config_.swing_left < bar_count_; ++i) {
        if (isSwingHigh(i)) {
            price = newest(i).high;
            return true;
        }
    }
    return false;
}

bool M5StructureEngine::latestSwingLow(double& price) const noexcept {
    const std::size_t start = config_.swing_right + 1U;
    for (std::size_t i = start; i + config_.swing_left < bar_count_; ++i) {
        if (isSwingLow(i)) {
            price = newest(i).low;
            return true;
        }
    }
    return false;
}

bool M5StructureEngine::isDisplacement(
    const Bar& bar,
    const Direction direction,
    const double current_atr) const noexcept {

    if (current_atr <= 0.0) return false;
    const double body = std::abs(bar.close - bar.open);
    const double range = bar.high - bar.low;
    if (range <= 0.0) return false;

    const bool directional =
        (direction == Direction::Buy && bar.close > bar.open) ||
        (direction == Direction::Sell && bar.close < bar.open);

    return directional &&
           body >= current_atr * config_.displacement_body_atr_multiple &&
           body / range >= config_.displacement_min_body_ratio;
}

M5Confirmation M5StructureEngine::onClosedBar(
    const Bar& bar,
    const Direction expected_direction) noexcept {

    bars_[write_index_] = bar;
    write_index_ = (write_index_ + 1U) % BarCapacity;
    if (bar_count_ < BarCapacity) ++bar_count_;

    M5Confirmation result{};
    if (expected_direction == Direction::None ||
        bar_count_ < config_.atr_period + config_.swing_left + config_.swing_right + 2U) {
        last_confirmation_ = result;
        return result;
    }

    const double current_atr = atr(config_.atr_period, 1U);
    double broken_level = 0.0;
    double protected_swing = 0.0;

    if (expected_direction == Direction::Buy) {
        if (!latestSwingHigh(broken_level) || !latestSwingLow(protected_swing))
            return last_confirmation_ = result;
        result.structure_break = bar.close > broken_level;
    } else {
        if (!latestSwingLow(broken_level) || !latestSwingHigh(protected_swing))
            return last_confirmation_ = result;
        result.structure_break = bar.close < broken_level;
    }

    result.displacement = isDisplacement(bar, expected_direction, current_atr);

    if (result.structure_break && result.displacement) {
        result.valid = true;
        result.direction = expected_direction;
        result.broken_level = broken_level;
        result.protected_swing = protected_swing;
        result.displacement_body = std::abs(bar.close - bar.open);
        result.atr = current_atr;
        result.timestamp_ms = bar.timestamp_ms;
    }

    last_confirmation_ = result;
    return result;
}

void M5StructureEngine::reset() noexcept {
    write_index_ = 0U;
    bar_count_ = 0U;
    last_confirmation_ = {};
}

} // namespace xau
