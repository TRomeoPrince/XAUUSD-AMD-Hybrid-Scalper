#pragma once

#include <array>
#include <cstddef>

#include "AmdDetector.hpp"
#include "M5StructureEngine.hpp"
#include "MarketTypes.hpp"
#include "SetupEngine.hpp"

namespace xau {

class TradingEngine {
public:
    static constexpr std::size_t TickBufferSize = 100;

    TradingEngine(AmdConfig amd_config = {}, M5Config m5_config = {}) noexcept;

    void onTick(const Tick& tick) noexcept;
    AmdEvent onClosedM15Bar(const Bar& bar) noexcept;
    SetupCandidate onClosedM5Bar(const Bar& bar) noexcept;

    [[nodiscard]] std::size_t tickCount() const noexcept;
    [[nodiscard]] StrategyState state() const noexcept;
    [[nodiscard]] const Range& activeM15Range() const noexcept;
    [[nodiscard]] const Manipulation& activeManipulation() const noexcept;
    [[nodiscard]] const M5Confirmation& m5Confirmation() const noexcept;
    [[nodiscard]] const SetupCandidate& latestSetup() const noexcept;
    [[nodiscard]] Signal evaluate() noexcept;

private:
    [[nodiscard]] const Tick& tickFromNewest(std::size_t offset) const noexcept;

    std::array<Tick, TickBufferSize> ticks_{};
    std::size_t write_index_{0};
    std::size_t tick_count_{0};
    AmdDetector amd_detector_;
    M5StructureEngine m5_engine_;
    SetupEngine setup_engine_;
    M5Confirmation m5_confirmation_{};
    SetupCandidate latest_setup_{};
};

} // namespace xau
