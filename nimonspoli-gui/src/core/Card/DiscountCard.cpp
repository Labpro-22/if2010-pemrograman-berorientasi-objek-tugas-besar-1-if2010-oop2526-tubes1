#include "DiscountCard.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

DiscountCard::DiscountCard()
{
}

DiscountCard::DiscountCard(const string &type, const string &description, bool used, double discountPercent, int duration) : SkillCard(type, description, used), discountPercent(discountPercent), duration(duration)
{
}

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