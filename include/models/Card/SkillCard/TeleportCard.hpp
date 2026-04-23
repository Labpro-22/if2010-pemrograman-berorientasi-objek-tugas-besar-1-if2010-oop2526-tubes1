#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"

class TeleportCard : public SkillCard {
    private:
        int targetIndex;
    public:
        TeleportCard(int targetIndex);
        ~TeleportCard();
        void activate(GameState&);
        const string getName() const;
        const string getDescription() const;
};
