#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class MoveCard : public SkillCard {
    public:
        MoveCard();
        ~MoveCard();
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
