#pragma once
#include "models/Board/CardDeck.hpp"
#include "core/GameEngine.hpp"

class ChanceCard {
    public:
        virtual void activate(GameState&) = 0;
        virtual const string getName() const = 0;
        virtual const string getDescription() const = 0;
};

