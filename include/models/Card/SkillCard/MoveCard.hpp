#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class MoveCard : public SkillCard {
    public:
        MoveCard() = default;
        ~MoveCard() = default;
        void activate(GameState&);
        const string getName() const;
        const string getDescription() const;
};
