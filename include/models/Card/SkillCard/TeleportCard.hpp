#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "iostream"
using namespace std;

class TeleportCard : public SkillCard {
    private:
        int targetIndex;
    public:
        TeleportCard(int targetIndex);
        ~TeleportCard();
        void activate(GameEngine&);
        string getName();
        string getDescription();
};
