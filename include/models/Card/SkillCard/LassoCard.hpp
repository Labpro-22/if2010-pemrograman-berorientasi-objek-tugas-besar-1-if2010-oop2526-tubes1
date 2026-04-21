#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class LassoCard : public SkillCard {
    public:
        LassoCard();
        ~LassoCard();
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
