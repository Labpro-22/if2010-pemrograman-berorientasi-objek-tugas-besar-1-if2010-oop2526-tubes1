#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class BackwardMoveCard : public ChanceCard{
    public:
        BackwardMoveCard();
        ~BackwardMoveCard();
        string getName();
        string getDescription();
        void activate();
};

