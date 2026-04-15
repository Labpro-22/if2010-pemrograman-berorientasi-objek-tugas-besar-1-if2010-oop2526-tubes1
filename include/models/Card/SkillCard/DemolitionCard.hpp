#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DemolitionCard : public SkillCard {
    public:
        DemolitionCard();
        ~DemolitionCard();
        void activate();
        string getName();
        string getDescription();
};
