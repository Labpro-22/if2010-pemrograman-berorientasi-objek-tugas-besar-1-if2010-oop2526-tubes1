#include "../../include/models/Effect.hpp"
#include "../../include/models/Player.hpp"

// Effect base class
void Effect::onTurnStart(Player& p) {}
void Effect::onTurnEnd(Player& p) {}

bool Effect::blocksPayment() const {
    return false;
}

float Effect::modifyPayment(float amount) const {
    return amount;
}

// ShieldEffect
ShieldEffect::ShieldEffect(int duration) : turns_left(duration) {}

void ShieldEffect::onTurnEnd(Player& p) {
    if (turns_left > 0) {
        --turns_left;
    }
}

bool ShieldEffect::isExpired() const {
    return turns_left <= 0;
}

bool ShieldEffect::blocksPayment() const {
    return turns_left > 0;
}

// DiscountEffect
DiscountEffect::DiscountEffect(int percent, int duration) 
    : percent(percent), turns_left(duration) {}

void DiscountEffect::onTurnEnd(Player& p) {
    if (turns_left > 0) {
        --turns_left;
    }
}

bool DiscountEffect::isExpired() const {
    return turns_left <= 0;
}

float DiscountEffect::modifyPayment( float amount) const {
    float multiplier = 1.0f - (percent / 100.0f);
    if (multiplier < 0.0f) {
        multiplier = 0.0f;
    }
    return amount * multiplier;
}