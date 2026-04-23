#pragma once
#include "models/Card/ChanceCard/ChanceCard.hpp"

class PrisonCard : public ChanceCard{
    public:
        PrisonCard() = default;
        ~PrisonCard() = default;
        const string getName() const;
        const string getDescription() const;
        void activate(GameState&);
};
