#pragma once

#include "MarketTypes.hpp"
#include "M5StructureEngine.hpp"

namespace xau {

enum class RejectionReason : std::uint8_t {
    None = 0,
    NoM15Manipulation,
    NoM5Confirmation,
    WrongDirection,
    RetestNotReached,
    TooFarFromPoi
};

struct SetupCandidate {
    bool ready{false};
    Direction direction{Direction::None};
    EntryTrigger trigger{EntryTrigger::None};
    RejectionReason rejection{RejectionReason::None};
    double entry{0.0};
    double stop{0.0};
    double reference_level{0.0};
    double protected_swing{0.0};
    std::int64_t timestamp_ms{0};
};

class SetupEngine {
public:
    SetupCandidate evaluateRetest(
        const Bar& m5_bar,
        const Manipulation& manipulation,
        const M5Confirmation& confirmation,
        double max_distance) const noexcept;
};

} // namespace xau
