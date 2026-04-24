#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Board/Board.hpp"

class MoveCard : public SkillCard {
    public:
        MoveCard() = default;
        ~MoveCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
