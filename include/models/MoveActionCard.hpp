#include "ActionCard.hpp"
#include <random>

class MoveActionCard:public ActionCard{
    private:
        static std::random_device rd;
        static std::mt19937 gen;
        int walkDistance;
    public:
        MoveActionCard();
        void onDraw(Player& p, Board& b, std::vector<Player>& all) override;
};