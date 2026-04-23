#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class BackwardMoveCard : public ChanceCard{
    public:
        BackwardMoveCard() = default;
        ~BackwardMoveCard() = default;
        const string getName() const;
        const string getDescription() const;
        void activate(GameState&);
};

