#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DiscountCard : public SkillCard {
    public:
        DiscountCard() = default;
        ~DiscountCard() = default;
        void activate(GameState&);
        const string getName() const;
        const string getDescription() const;
};
