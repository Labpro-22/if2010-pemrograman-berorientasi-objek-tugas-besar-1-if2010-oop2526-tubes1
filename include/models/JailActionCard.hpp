#include "ActionCard.hpp"
#include <random>
class JailActionCard : public ActionCard { 
    private:
        static std::random_device rd;
        static std::mt19937 gen;
        int sentenceLength;
    public:
        JailActionCard();
        void onDraw(Player& p, Board& b, std::vector<Player>& all) override;
        

};