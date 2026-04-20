#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class PrisonCard : public ChanceCard{
    public:
        PrisonCard();
        ~PrisonCard();
        string getName();
        string getDescription();
        void activate();
};
