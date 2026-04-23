#include "ActionCard.hpp"

class StationActionCard : public ActionCard {
 
    public:
        void onDraw(Player& p, Board& b, std::vector<Player>& all) override;


};