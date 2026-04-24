#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class ShieldCard : public SkillCard {
    public:
        ShieldCard() = default;
        ~ShieldCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
