#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Board/Board.hpp"

class StationCard : public ChanceCard{
    public:
        StationCard() = default;
        ~StationCard() = default;
        const string getName() const;
        const string getDescription() const;
        void activate(SkillContext& ctx);
};
