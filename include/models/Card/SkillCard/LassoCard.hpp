#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Board/Board.hpp"

class LassoCard : public SkillCard {
    public:
        LassoCard() = default;
        ~LassoCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
