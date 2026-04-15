#include "models/Board/CardDeck.hpp"

class SkillCard {
    private:
        
    public:
        SkillCard();
        ~SkillCard();
        virtual void activate();
        virtual void getName();
        virtual void getDescription();
};

