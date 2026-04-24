#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DiscountCard : public SkillCard {
    public:
        DiscountCard() = default;
        ~DiscountCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
