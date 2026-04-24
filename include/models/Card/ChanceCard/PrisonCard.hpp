#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Board/Board.hpp"

class PrisonCard : public ChanceCard{
    public:
        PrisonCard() = default;
        ~PrisonCard() = default;
        const string getName() const;
        const string getDescription() const;
        void activate(SkillContext& ctx);
};
