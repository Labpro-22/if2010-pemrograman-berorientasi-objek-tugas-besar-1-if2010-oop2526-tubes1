#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class DemolitionCard : public SkillCard {
    public:
        DemolitionCard() = default;
        ~DemolitionCard() = default;
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
