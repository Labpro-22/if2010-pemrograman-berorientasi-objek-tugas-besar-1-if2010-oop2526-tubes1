#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class MoveCard : public SkillCard {
    public:
        MoveCard();
        ~MoveCard();
        void activate();
        string getName();
        string getDescription();
};
