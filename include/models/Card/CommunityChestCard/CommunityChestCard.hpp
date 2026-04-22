#pragma once
#include <string>
#include "models/Board/CardDeck.hpp"
using namespace std;

class GameEngine;
class CommunityChestCard {
    public:
        virtual void activate(GameEngine& ge) = 0;
        virtual string getName() = 0;
        virtual string getDescription() = 0;
};
 