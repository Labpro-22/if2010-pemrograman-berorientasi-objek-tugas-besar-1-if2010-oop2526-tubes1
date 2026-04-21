#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class StationCard : public ChanceCard{
    public:
        StationCard() = default;
        ~StationCard() = default;
        string getName();
        string getDescription();
        void activate(GameEngine&);
};
