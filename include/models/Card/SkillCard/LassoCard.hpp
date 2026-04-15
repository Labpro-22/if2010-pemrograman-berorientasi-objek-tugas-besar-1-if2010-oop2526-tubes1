#include "models/Card/SkillCard/SkillCard.hpp"

class LassoCard : public SkillCard {
    public:
        LassoCard();
        ~LassoCard();
        void activate();
        void getDescription();
};
