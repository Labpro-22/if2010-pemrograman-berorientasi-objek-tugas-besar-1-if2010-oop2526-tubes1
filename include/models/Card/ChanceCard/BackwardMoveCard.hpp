#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class BackwardMoveCard : public ChanceCard{
    public:
        BackwardMoveCard() = default;
        ~BackwardMoveCard() = default;
        string getName();
        string getDescription();
        void activate(GameEngine&);
};

