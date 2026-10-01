#include "M5StructureEngine.hpp"
#include "SetupEngine.hpp"

#include <cassert>
#include <iostream>

using namespace xau;

int main() {
    M5Config cfg{};
    cfg.swing_left = 1;
    cfg.swing_right = 1;
    cfg.atr_period = 3;
    cfg.displacement_body_atr_multiple = 0.50;
    cfg.displacement_min_body_ratio = 0.60;

    M5StructureEngine engine(cfg);
    std::int64_t t = 0;

    // Warm-up + a clear swing high near 2002 and swing low near 1999.
    engine.onClosedBar({2000.0,2001.0,1999.5,2000.5,100,t+=300000}, Direction::Buy);
    engine.onClosedBar({2000.5,2002.0,2000.0,2001.0,100,t+=300000}, Direction::Buy);
    engine.onClosedBar({2001.0,2001.5,1999.0,1999.8,100,t+=300000}, Direction::Buy);
    engine.onClosedBar({1999.8,2001.0,1999.4,2000.6,100,t+=300000}, Direction::Buy);
    engine.onClosedBar({2000.6,2001.2,2000.0,2000.8,100,t+=300000}, Direction::Buy);

    const auto confirmation =
        engine.onClosedBar({2000.8,2003.0,2000.6,2002.8,150,t+=300000}, Direction::Buy);

    assert(confirmation.valid);
    assert(confirmation.direction == Direction::Buy);
    assert(confirmation.structure_break);
    assert(confirmation.displacement);

    Manipulation manipulation{};
    manipulation.valid = true;
    manipulation.expected_distribution = Direction::Buy;

    SetupEngine setups;
    const Bar retest{2002.7,2002.8,2001.9,2002.2,120,t+=300000};
    const auto candidate = setups.evaluateRetest(
        retest, manipulation, confirmation, 0.5);

    assert(candidate.ready);
    assert(candidate.trigger == EntryTrigger::AmdStructureRetest);
    assert(candidate.stop == confirmation.protected_swing);

    std::cout << "M5 confirmation smoke test passed\n";
    return 0;
}
