#include "models/Card/SkillCard/DiscountCard.hpp"

void DiscountCard::activate(SkillContext& ctx) {
    random_device random;
    mt19937 g(random());
    uniform_int_distribution<> number(1,100);

    int discountPercent = number(g);
    ctx.getCurrentPlayer().setDiscountValue(discountPercent);
    ctx.getCurrentPlayer().setDiscountTurnLeft(1);
}

const string DiscountCard::getName() const {
    return "DiscountCard";
}

const string DiscountCard::getDescription() const {
    return "Memberikan diskon dengan persentase acak kepada pemain saat kartu didapatkan. Masa berlaku DiscountCard adalah 1 giliran.";
}