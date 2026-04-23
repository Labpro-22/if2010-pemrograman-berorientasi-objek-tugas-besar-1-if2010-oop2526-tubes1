#include "MoveActionCard.hpp"
#include "Player.hpp"
#include "Board.hpp"

std::random_device MoveActionCard::rd;
std::mt19937 MoveActionCard::gen(MoveActionCard::rd());

MoveActionCard::MoveActionCard(){
    std::uniform_int_distribution<> move(1,5);
    std::uniform_int_distribution<> forwardorbackwardhmmiwonder(0,1);
    walkDistance = forwardorbackwardhmmiwonder(gen)?move(gen):move(gen)*(-1);
}

void MoveActionCard::onDraw(Player& p, Board& b, std::vector<Player>& all){
    p.movePlayer(walkDistance);
}