#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "iostream"
using namespace std;

class TeleportCard : public SkillCard {
    public:
        TeleportCard();
        ~TeleportCard();
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
