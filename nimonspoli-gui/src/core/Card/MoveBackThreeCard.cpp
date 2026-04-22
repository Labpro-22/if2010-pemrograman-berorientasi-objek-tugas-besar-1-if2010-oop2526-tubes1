#include "MoveBackThreeCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

MoveToBackThreeCard::MoveToBackThreeCard()
{
}

MoveToBackThreeCard::MoveToBackThreeCard(const string &type, const string &description) : ChanceCard(type, description)
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