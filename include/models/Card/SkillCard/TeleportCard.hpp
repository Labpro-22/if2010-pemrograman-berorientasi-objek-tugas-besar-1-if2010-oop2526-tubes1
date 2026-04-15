#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class TeleportCard : public SkillCard {
    public:
        TeleportCard();
        ~TeleportCard();
        void activate();
        string getName();
        string getDescription();
};
