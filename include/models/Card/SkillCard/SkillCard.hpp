#pragma once
#include "models/Board/CardDeck.hpp"
#include "core/GameException.hpp"
#include "core/SkillContext.hpp"

class SkillCard {
    public:
        virtual ~SkillCard() = default;
        virtual void activate(SkillContext& ctx) = 0;
        virtual const string getName() const = 0;
        virtual const string getDescription() const = 0;
};

