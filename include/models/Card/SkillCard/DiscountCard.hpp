#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DiscountCard : public SkillCard {
    public:
        DiscountCard() = default;
        ~DiscountCard() = default;
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
