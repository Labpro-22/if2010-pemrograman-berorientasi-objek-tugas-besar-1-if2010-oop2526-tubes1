#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DemolitionCard : public SkillCard {
    public:
        DemolitionCard() = default;
        ~DemolitionCard() = default;
        void activate(GameState&);
        const string getName() const;
        const string getDescription() const;
};
