#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class StationCard : public ChanceCard{
    public:
        StationCard();
        ~StationCard();
        string getName();
        string getDescription();
        void activate();
};
