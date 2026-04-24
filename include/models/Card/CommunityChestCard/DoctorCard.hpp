#pragma once
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
 
class DoctorCard : public CommunityChestCard {
    public:
        DoctorCard();
        ~DoctorCard();
        string getName();
        string getDescription();
        void activate(SkillContext& ctx);
};
 