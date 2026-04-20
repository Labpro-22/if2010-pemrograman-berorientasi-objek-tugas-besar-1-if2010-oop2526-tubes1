#pragma once
#include "models/Board/CardDeck.hpp"

class ChanceCard {
    public:
        virtual void activate() = 0;
        virtual string getName() = 0;
        virtual string getDescription() = 0;
};

