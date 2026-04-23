#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class StationCard : public ChanceCard{
    public:
        StationCard() = default;
        ~StationCard() = default;
        const string getName() const;
        const string getDescription() const;
        void activate(GameState&);
};
