#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class LassoCard : public SkillCard {
    public:
        LassoCard() = default;
        ~LassoCard() = default;
        void activate(GameState&);
        const string getName() const;
        const string getDescription() const;
};
