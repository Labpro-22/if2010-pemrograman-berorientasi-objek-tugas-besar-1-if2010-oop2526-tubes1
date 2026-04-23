#pragma once
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"

class BirthdayCard : public CommunityChestCard {
    public:
        BirthdayCard();
        ~BirthdayCard();
        string getName();
        string getDescription();
        void activate(SkillContext& ctx);
};