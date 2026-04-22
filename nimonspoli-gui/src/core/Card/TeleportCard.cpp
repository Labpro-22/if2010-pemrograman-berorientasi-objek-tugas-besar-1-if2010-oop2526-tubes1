#include "TeleportCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

class Board;

TeleportCard::TeleportCard()
{
}

TeleportCard::TeleportCard(const string &type, const string &description, bool used) : SkillCard(type, description, used), targetPosition(-999)
{
}

TeleportCard::~TeleportCard()
{
}

void TeleportCard::setTargetPosition(int pos, GameState &gs)
{
    int boardSize = gs.getBoard()->getSize();
    if (pos >= 0 && pos < boardSize)
        targetPosition = pos;
}

int TeleportCard::getTargetPosition() const { return targetPosition; }

void TeleportCard::execute(Player &p, GameState &gs)
{
    if (targetPosition == -999)
        return;
    p.setPosition(targetPosition);
}