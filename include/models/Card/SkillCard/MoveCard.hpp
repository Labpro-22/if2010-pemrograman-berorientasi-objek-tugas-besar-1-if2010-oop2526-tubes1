#include "models/Card/SkillCard/SkillCard.hpp"

class MoveCard : public SkillCard {
    public:
        MoveCard();
        ~MoveCard();
        void activate();
        void getDescription();
};
