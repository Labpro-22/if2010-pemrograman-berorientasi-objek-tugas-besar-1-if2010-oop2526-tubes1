#include "MoveBackThreeCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

MoveToBackThreeCard::MoveToBackThreeCard() : ChanceCard("Mundur 3 petak.")
{
}

MoveToBackThreeCard::~MoveToBackThreeCard()
{
}

void MoveToBackThreeCard::execute(Player &p, GameState &gs)
{
    int boardSize = gs.getBoard()->getSize();
    int newPos = (p.getPosition() - 3 + boardSize) % boardSize;
    p.setPosition(newPos);
}