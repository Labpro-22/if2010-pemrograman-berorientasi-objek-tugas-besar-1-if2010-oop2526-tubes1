#include "LassoCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

class Board;

LassoCard::LassoCard()
{
}

LassoCard::LassoCard(const string &type, const string &description, bool used) : SkillCard(type, description, used)
{
}

LassoCard::~LassoCard()
{
}

void LassoCard::execute(Player &p, GameState &gs)
{
    int currPos = p.getPosition();
    int boardSize = gs.getBoard()->getSize();
    vector<Player *> players = gs.getActivePlayers();

    Player *target = nullptr;
    int minSteps = boardSize + 1;

    for (Player *other : players)
    {
        if (other == &p)
            continue;
        int steps = (other->getPosition() - currPos + boardSize) % boardSize;
        if (steps > 0 && steps < minSteps)
        {
            minSteps = steps;
            target = other;
        }
    }

    if (target)
        target->setPosition(currPos);
}