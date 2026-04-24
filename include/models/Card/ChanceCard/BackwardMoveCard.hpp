#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Board/Board.hpp"

class BackwardMoveCard : public ChanceCard{
    public:
        BackwardMoveCard() = default;
        ~BackwardMoveCard() = default;
        const string getName() const;
        const string getDescription() const;
        void activate(SkillContext& ctx);
};

