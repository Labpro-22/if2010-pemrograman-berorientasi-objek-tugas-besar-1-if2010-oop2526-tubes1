#include "models/Card/SkillCard/DiscountCard.hpp"

void DiscountCard::activate(GameState& state) {
    random_device random;
    mt19937 g(random());
    uniform_int_distribution<> number(1,100);

    int discountPercent = number(g);
    state.getCurrentPlayer().setDiscountValue(discountPercent);
    state.getCurrentPlayer().setDiscountTurnLeft(1);
}

const string DiscountCard::getName() const {
    return "DiscountCard";
}

const string DiscountCard::getDescription() const {
    return "Memberikan diskon dengan persentase acak kepada pemain saat kartu didapatkan. Masa berlaku DiscountCard adalah 1 giliran.";
}