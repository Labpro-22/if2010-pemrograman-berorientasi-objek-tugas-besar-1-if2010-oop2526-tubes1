#ifndef DISCOUNT_CARD_HPP
#define DISCOUNT_CARD_HPP

#include "SkillCard.hpp"
using namespace std;

class DiscountCard : public SkillCard
{
private:
    double discountPercent;
    int duration;

public:
    DiscountCard();
    DiscountCard(double discountPercent, int duration);
    ~DiscountCard();
    int getDuration();
    void decreaseDuration();
    void execute(Player &p, GameState &gs) override;
};

#endif