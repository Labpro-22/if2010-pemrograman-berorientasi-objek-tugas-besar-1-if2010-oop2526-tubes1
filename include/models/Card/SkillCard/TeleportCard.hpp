#pragma once
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Board/Board.hpp"

class TeleportCard : public SkillCard {
    private:
        int targetIndex;
    public:
        TeleportCard(int targetIndex);
        ~TeleportCard();
        void activate(SkillContext& ctx);
        const string getName() const;
        const string getDescription() const;
};
