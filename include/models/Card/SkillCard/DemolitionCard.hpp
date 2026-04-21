#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DemolitionCard : public SkillCard {
    public:
        DemolitionCard() = default;
        ~DemolitionCard() = default;
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
