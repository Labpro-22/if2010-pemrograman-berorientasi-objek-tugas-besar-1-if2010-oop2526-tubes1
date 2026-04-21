#include "models/Card/SkillCard/DiscountCard.hpp"

DiscountCard::DiscountCard() {

}

DiscountCard::~DiscountCard() {

}

void DiscountCard::activate(GameEngine& ge) {
    random_device random;
    mt19937 g(random());
    uniform_int_distribution<> number(1,100);

    int discountPercent = number(g);

}

string DiscountCard::getName() {
    return "DiscountCard";
}

string DiscountCard::getDescription() {
    return "Memberikan diskon dengan persentase acak kepada pemain saat kartu didapatkan. Masa berlaku DiscountCard adalah 1 giliran.";
}