#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class ShieldCard : public SkillCard {
    public:
        ShieldCard() = default;
        ~ShieldCard() = default;
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
