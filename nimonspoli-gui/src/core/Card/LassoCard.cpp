#include "LassoCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../GameMaster/GameMaster.hpp"
#include "../Board/Board.hpp"

LassoCard::LassoCard() : SkillCard("Tarik lawan ke petakmu", "LassoCard")
{
}

LassoCard::LassoCard(const string &type, const string &description, bool used) : SkillCard(type, description, used)
{
}

LassoCard::~LassoCard()
{
}

void LassoCard::setTargetPlayer(Player *target)
{
    targetPlayer = target;
}

void LassoCard::execute(Player &p, GameState &gs)
{
    int currPos = p.getPosition();
    Player *target = targetPlayer;

    if (!target)
    {
        int boardSize = gs.getBoard()->getSize();
        vector<Player *> players = gs.getActivePlayers();
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
    }

    if (target)
    {
        GameMaster *gm = gs.getGameMaster();
        if (gm)
            gm->teleportPlayer(target, currPos);
    }

    targetPlayer = nullptr;
}

string LassoCard::successMessage() const
{
    return "LassoCard digunakan. Pilih pemain lawan yang ingin ditarik ke petak Anda.";
}