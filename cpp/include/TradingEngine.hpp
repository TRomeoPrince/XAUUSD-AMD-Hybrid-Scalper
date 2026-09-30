#pragma once

#include <array>
#include <cstddef>

#include "MarketTypes.hpp"

namespace xau {

class TradingEngine {
public:
    static constexpr std::size_t TickBufferSize = 100;

    TradingEngine() noexcept = default;

    void onTick(const Tick& tick) noexcept;

    [[nodiscard]] std::size_t tickCount() const noexcept;
    [[nodiscard]] StrategyState state() const noexcept;
    [[nodiscard]] Signal evaluate() noexcept;

private:
    [[nodiscard]] const Tick& tickFromNewest(std::size_t offset) const noexcept;
    void setState(StrategyState next_state) noexcept;

    std::array<Tick, TickBufferSize> ticks_{};
    std::size_t write_index_{0};
    std::size_t tick_count_{0};
    StrategyState state_{StrategyState::Searching};
};

} // namespace xau
