#include "DiscountCard.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

DiscountCard::DiscountCard() : SkillCard("Diskon " + to_string(rand() % 50 + 10) + "%", "DiscountCard"), discountPercent(rand() % 50 + 10), duration(1) {}

DiscountCard::DiscountCard(double discountPercent, int duration) : SkillCard("Diskon " + to_string(discountPercent) + "%", "DiscountCard"), discountPercent(discountPercent), duration(duration) {}

DiscountCard::~DiscountCard()
{
}

int DiscountCard::getDuration()
{
    return duration;
}

void DiscountCard::decreaseDuration()
{
    duration -= 1;
}

void DiscountCard::execute(Player &p, GameState &gs)
{
    double discountPercent = rand();
    p.setDiscount(discountPercent);
}