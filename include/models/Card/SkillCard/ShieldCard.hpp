#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class ShieldCard : public SkillCard {
    public:
        ShieldCard();
        ~ShieldCard();
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
