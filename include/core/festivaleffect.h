#pragma once

namespace Nimonspoli {
class FestivalEffect {
public:
    FestivalEffect() = default;
    bool isActive()      const { return duration_ > 0; }
    int  multiplier()    const { return multiplier_; }
    int  duration()      const { return duration_; }
    int  boostCount()    const { return boostCount_; }
    int boost() {
        if (boostCount_ < 3) {
            multiplier_ *= 2;
            ++boostCount_;
        }
        duration_ = 3;
        return multiplier_;
    }
    void tick() {
        if (duration_ > 0) --duration_;
        if (duration_ == 0) reset();
    }

    void reset() {
        multiplier_ = 1;
        duration_   = 0;
        boostCount_ = 0;
    }

    // For save/load
    void setState(int mult, int dur, int boosts) {
        multiplier_ = mult;
        duration_   = dur;
        boostCount_ = boosts;
    }

private:
    int multiplier_  = 1;
    int duration_    = 0;
    int boostCount_  = 0;
};

} 