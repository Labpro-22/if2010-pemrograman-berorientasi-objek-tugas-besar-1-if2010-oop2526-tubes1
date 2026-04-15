#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DiscountCard : public SkillCard {
    public:
        DiscountCard();
        ~DiscountCard();
        void activate();
        string getName();
        string getDescription();
};
