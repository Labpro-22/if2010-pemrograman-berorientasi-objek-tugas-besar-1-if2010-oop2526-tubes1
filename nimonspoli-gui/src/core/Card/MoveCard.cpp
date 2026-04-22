#include "MoveCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

class Board;

MoveCard::MoveCard()
{
}

MoveCard::MoveCard(const string &type, const string &description, bool used, int steps) : SkillCard(type, description, used), steps(steps)
{
}

MoveCard::~MoveCard()
{
}

int MoveCard::getSteps() const
{
    return steps;
}

void MoveCard::execute(Player &p, GameState &gs)
{
    int boardSize = gs.getBoard()->getSize();
    int randomNum = (p.getPosition() + rand()) % boardSize;
    p.setPosition(randomNum);
}
