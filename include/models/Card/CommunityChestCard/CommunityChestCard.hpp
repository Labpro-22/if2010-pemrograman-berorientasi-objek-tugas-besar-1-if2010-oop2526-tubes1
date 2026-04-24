#pragma once
#include <string>
#include "models/Board/CardDeck.hpp"
#include "core/SkillContext.hpp"
using namespace std;

class CommunityChestCard {
    public:
        virtual ~CommunityChestCard() = default;
        virtual void activate(SkillContext& ctx) = 0;
        virtual string getName() = 0;
        virtual string getDescription() = 0;
};
 