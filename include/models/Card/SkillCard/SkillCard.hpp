#pragma once
#include "models/Board/CardDeck.hpp"

class SkillCard {
    public:
        SkillCard();
        ~SkillCard();
        virtual void activate();
        virtual string getName();
        virtual string getDescription();
};

