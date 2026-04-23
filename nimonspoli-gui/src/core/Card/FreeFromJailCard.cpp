#include "FreeFromJailCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"

FreeFromJailCard::FreeFromJailCard()
{
}

FreeFromJailCard::FreeFromJailCard(const string &type, const string &description) : ChanceCard(type, description)
{
}

FreeFromJailCard::~FreeFromJailCard()
{
}

void FreeFromJailCard::execute(Player &p, GameState &gs)
{
    if (p.isInJail())
    {
        p.releaseFromJail();
    }
}