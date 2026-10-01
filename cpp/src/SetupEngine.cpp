#include "SetupEngine.hpp"

#include <cmath>

namespace xau {

SetupCandidate SetupEngine::evaluateRetest(
    const Bar& bar,
    const Manipulation& manipulation,
    const M5Confirmation& confirmation,
    const double max_distance) const noexcept {

    SetupCandidate out{};
    out.timestamp_ms = bar.timestamp_ms;

    if (!manipulation.valid) {
        out.rejection = RejectionReason::NoM15Manipulation;
        return out;
    }
    if (!confirmation.valid) {
        out.rejection = RejectionReason::NoM5Confirmation;
        return out;
    }
    if (confirmation.direction != manipulation.expected_distribution) {
        out.rejection = RejectionReason::WrongDirection;
        return out;
    }

    out.direction = confirmation.direction;
    out.reference_level = confirmation.broken_level;
    out.protected_swing = confirmation.protected_swing;

    const bool touched =
        bar.low <= confirmation.broken_level && bar.high >= confirmation.broken_level;

    if (!touched) {
        const double distance =
            confirmation.direction == Direction::Buy
                ? bar.low - confirmation.broken_level
                : confirmation.broken_level - bar.high;

        out.rejection = distance > max_distance
            ? RejectionReason::TooFarFromPoi
            : RejectionReason::RetestNotReached;
        return out;
    }

    const bool held =
        confirmation.direction == Direction::Buy
            ? bar.close >= confirmation.broken_level
            : bar.close <= confirmation.broken_level;

    if (!held) {
        out.rejection = RejectionReason::RetestNotReached;
        return out;
    }

    out.ready = true;
    out.trigger = EntryTrigger::AmdStructureRetest;
    out.entry = bar.close;
    out.stop = confirmation.protected_swing;
    return out;
}

} // namespace xau
