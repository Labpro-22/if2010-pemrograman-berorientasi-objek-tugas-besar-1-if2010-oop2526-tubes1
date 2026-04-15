#include "models/Card/SkillCard/DiscountCard.hpp"

DiscountCard::DiscountCard() {

}

DiscountCard::~DiscountCard() {

}

void DiscountCard::activate() {
     // TODO: 
}

string DiscountCard::getName() {
    return "DiscountCard";
}

string DiscountCard::getDescription() {
    return "Memberikan diskon dengan persentase acak kepada pemain saat kartu didapatkan. Masa berlaku DiscountCard adalah 1 giliran.";
}