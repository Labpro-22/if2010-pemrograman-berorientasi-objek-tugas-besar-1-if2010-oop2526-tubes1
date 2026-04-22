#pragma once
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
 
class CampaignCard : public CommunityChestCard {
    public:
        CampaignCard();
        ~CampaignCard();
        string getName();
        string getDescription();
        void activate(GameEngine& ge);
};
 