#include "LassoCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../GameMaster/GameMaster.hpp"
#include "../Board/Board.hpp"
#include "../Board/Board.hpp"

class Board;
class GameMaster;

LassoCard::LassoCard() : SkillCard("Tarik lawan ke petakmu", "LassoCard")
{
}

LassoCard::LassoCard(const string &type, const string &description, bool used) : SkillCard(type, description, used), targetPlayerUsername("")
{
}

LassoCard::~LassoCard()
{
}

void LassoCard::setTargetPlayerUsername(const string &target)
{
    this->targetPlayerUsername = target;
}

void LassoCard::execute(Player &p, GameState &gs)
{
    Board *board = gs.getBoard();
    if (!board)
        return;

    int currPos = p.getPosition();
    int boardSize = board->getSize();

    Player *target = nullptr;

    vector<Player *> players = gs.getActivePlayers();

    if (!targetPlayerUsername.empty())
    {
        for (Player *other : players)
        {
            if (!other)
                continue;

            if (other == &p)
                continue;
            if (other->isInJail())
                break;

            if (other->getUsername() == targetPlayerUsername)
            {
                int steps = (other->getPosition() - currPos + boardSize) % boardSize;

                // Lasso hanya valid untuk pemain lawan yang ada di depan
                if (steps > 0)
                    target = other;

                break;
            }
        }
    }
    else
    {
        int minSteps = boardSize + 1;

        for (Player *other : players)
        {
            if (!other || other == &p)
                continue;

            int steps = (other->getPosition() - currPos + boardSize) % boardSize;

            if (steps > 0 && steps < minSteps)
            {
                minSteps = steps;
                target = other;
            }
        }
    }

    if (!target)
        return;

    GameMaster *gm = gs.getGameMaster();
    if (!gm)
        return;

    gm->teleportPlayer(target, currPos, false);

    targetPlayerUsername = "";
    markUsed();
}

string LassoCard::successMessage() const
{
    return "LassoCard digunakan. Pilih pemain lawan yang ingin ditarik ke petak Anda.";
}