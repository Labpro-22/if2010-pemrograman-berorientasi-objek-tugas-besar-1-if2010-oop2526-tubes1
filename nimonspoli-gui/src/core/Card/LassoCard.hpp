#ifndef LASSO_CARD_HPP
#define LASSO_CARD_HPP

#include "SkillCard.hpp"
using namespace std;

class LassoCard : public SkillCard
{
private:
    Player *targetPlayer = nullptr;

public:
    LassoCard();
    LassoCard(const string &type, const string &description, bool used);
    void setTargetPlayer(Player *target);
    ~LassoCard();
    void execute(Player &p, GameState &gs) override;
    string successMessage() const override;
};

#endif