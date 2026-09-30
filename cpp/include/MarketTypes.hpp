#pragma once

#include <cstdint>

namespace xau {

struct Tick {
    double bid{0.0};
    double ask{0.0};
    double volume{0.0};
    std::int64_t timestamp_ms{0};
};

struct Bar {
    double open{0.0};
    double high{0.0};
    double low{0.0};
    double close{0.0};
    double volume{0.0};
    std::int64_t timestamp_ms{0};
};

enum class Direction : std::uint8_t { None = 0, Buy = 1, Sell = 2 };
enum class SweepSide : std::uint8_t { None = 0, RangeHigh = 1, RangeLow = 2 };

enum class StrategyState : std::uint8_t {
    Searching = 0,
    AccumulationDetected,
    ManipulationDetected,
    AwaitingM5Confirmation,
    DistributionConfirmed,
    AwaitingRetest,
    EntryReady,
    TradeActive,
    Completed,
    Invalidated
};

enum class EntryTrigger : std::uint8_t {
    None = 0,
    AmdFvgRetest,
    AmdOrderBlockRetest,
    AmdStructureRetest,
    BreakoutRetest,
    AmdRejection
};

struct Range {
    bool valid{false};
    double high{0.0};
    double low{0.0};
    double midpoint{0.0};
    double width{0.0};
    std::int64_t start_timestamp_ms{0};
    std::int64_t end_timestamp_ms{0};
    std::uint32_t bars{0};
};

struct Manipulation {
    bool valid{false};
    SweepSide side{SweepSide::None};
    Direction expected_distribution{Direction::None};
    double extreme{0.0};
    double sweep_distance{0.0};
    std::int64_t timestamp_ms{0};
};

struct Signal {
    bool trigger_trade{false};
    Direction direction{Direction::None};
    EntryTrigger entry_trigger{EntryTrigger::None};
    double entry_price{0.0};
    double stop_loss{0.0};
    double take_profit{0.0};
    double confidence{0.0};
};

} // namespace xau
