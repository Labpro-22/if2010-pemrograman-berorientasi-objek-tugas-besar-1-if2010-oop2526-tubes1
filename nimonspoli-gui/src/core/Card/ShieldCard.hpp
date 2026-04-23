#ifndef SHIELD_CARD_HPP
#define SHIELD_CARD_HPP

#include "SkillCard.hpp"
using namespace std;

class ShieldCard : public SkillCard
{
public:
    ShieldCard();
    ~ShieldCard();
    void execute(Player &p, GameState &gs) override;
};

#endif