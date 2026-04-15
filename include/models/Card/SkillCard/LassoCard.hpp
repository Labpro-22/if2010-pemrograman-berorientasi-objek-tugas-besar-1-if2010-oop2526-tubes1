#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class LassoCard : public SkillCard {
    public:
        LassoCard();
        ~LassoCard();
        void activate();
        string getName();
        string getDescription();
};
