#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "MarketTypes.hpp"

namespace xau {

struct AmdConfig {
    std::size_t min_range_bars{4};
    std::size_t max_range_bars{12};
    double max_range_atr_multiple{1.50};
    double max_body_to_range_ratio{0.70};
    double min_sweep_atr_multiple{0.05};
    double max_sweep_atr_multiple{0.75};
    bool require_sweep_close_back_inside{true};
};

struct AmdEvent {
    StrategyState state{StrategyState::Searching};
    Range range{};
    Manipulation manipulation{};
    bool state_changed{false};
};

class AmdDetector {
public:
    static constexpr std::size_t BarCapacity = 64;

    explicit AmdDetector(AmdConfig config = {}) noexcept;

    AmdEvent onClosedM15Bar(const Bar& bar) noexcept;
    void reset() noexcept;

    [[nodiscard]] StrategyState state() const noexcept { return state_; }
    [[nodiscard]] const Range& activeRange() const noexcept { return range_; }
    [[nodiscard]] const Manipulation& manipulation() const noexcept { return manipulation_; }

private:
    [[nodiscard]] const Bar& newest(std::size_t offset) const noexcept;
    [[nodiscard]] double trueRange(std::size_t offset) const noexcept;
    [[nodiscard]] double averageTrueRange(std::size_t period, std::size_t offset = 0) const noexcept;
    [[nodiscard]] bool detectAccumulation(Range& candidate) const noexcept;
    [[nodiscard]] bool detectManipulation(Manipulation& candidate) const noexcept;
    void transition(StrategyState next, AmdEvent& event) noexcept;

    AmdConfig config_{};
    std::array<Bar, BarCapacity> bars_{};
    std::size_t write_index_{0};
    std::size_t bar_count_{0};
    StrategyState state_{StrategyState::Searching};
    Range range_{};
    Manipulation manipulation_{};
};

} // namespace xau
