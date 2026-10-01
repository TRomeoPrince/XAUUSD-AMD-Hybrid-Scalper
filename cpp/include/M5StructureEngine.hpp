#pragma once

#include <array>
#include <cstddef>

#include "MarketTypes.hpp"

namespace xau {

struct M5Config {
    std::size_t swing_left{2};
    std::size_t swing_right{2};
    std::size_t atr_period{14};
    double displacement_body_atr_multiple{0.80};
    double displacement_min_body_ratio{0.65};
    double max_retest_distance_atr{0.20};
};

struct M5Confirmation {
    bool valid{false};
    Direction direction{Direction::None};
    bool structure_break{false};
    bool displacement{false};
    double broken_level{0.0};
    double protected_swing{0.0};
    double displacement_body{0.0};
    double atr{0.0};
    std::int64_t timestamp_ms{0};
};

class M5StructureEngine {
public:
    static constexpr std::size_t BarCapacity = 128;

    explicit M5StructureEngine(M5Config config = {}) noexcept;

    M5Confirmation onClosedBar(const Bar& bar, Direction expected_direction) noexcept;
    void reset() noexcept;

    [[nodiscard]] const M5Confirmation& lastConfirmation() const noexcept {
        return last_confirmation_;
    }

private:
    [[nodiscard]] const Bar& newest(std::size_t offset) const noexcept;
    [[nodiscard]] double trueRange(std::size_t offset) const noexcept;
    [[nodiscard]] double atr(std::size_t period, std::size_t offset = 0) const noexcept;
    [[nodiscard]] bool isSwingHigh(std::size_t offset) const noexcept;
    [[nodiscard]] bool isSwingLow(std::size_t offset) const noexcept;
    [[nodiscard]] bool latestSwingHigh(double& price) const noexcept;
    [[nodiscard]] bool latestSwingLow(double& price) const noexcept;
    [[nodiscard]] bool isDisplacement(const Bar& bar, Direction direction, double current_atr) const noexcept;

    M5Config config_{};
    std::array<Bar, BarCapacity> bars_{};
    std::size_t write_index_{0};
    std::size_t bar_count_{0};
    M5Confirmation last_confirmation_{};
};

} // namespace xau
