#pragma once

#include <array>
#include <cstddef>

#include "AmdDetector.hpp"
#include "MarketTypes.hpp"

namespace xau {

class TradingEngine {
public:
    static constexpr std::size_t TickBufferSize = 100;

    explicit TradingEngine(AmdConfig amd_config = {}) noexcept;

    void onTick(const Tick& tick) noexcept;
    AmdEvent onClosedM15Bar(const Bar& bar) noexcept;

    [[nodiscard]] std::size_t tickCount() const noexcept;
    [[nodiscard]] StrategyState state() const noexcept;
    [[nodiscard]] const Range& activeM15Range() const noexcept;
    [[nodiscard]] const Manipulation& activeManipulation() const noexcept;
    [[nodiscard]] Signal evaluate() noexcept;

private:
    [[nodiscard]] const Tick& tickFromNewest(std::size_t offset) const noexcept;

    std::array<Tick, TickBufferSize> ticks_{};
    std::size_t write_index_{0};
    std::size_t tick_count_{0};
    AmdDetector amd_detector_;
};

} // namespace xau
