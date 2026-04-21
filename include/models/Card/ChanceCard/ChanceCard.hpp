#pragma once
#include "models/Board/CardDeck.hpp"
#include "core/GameEngine.hpp"

class ChanceCard {
    public:
        virtual void activate(GameEngine&) = 0;
        virtual string getName() = 0;
        virtual string getDescription() = 0;
};

