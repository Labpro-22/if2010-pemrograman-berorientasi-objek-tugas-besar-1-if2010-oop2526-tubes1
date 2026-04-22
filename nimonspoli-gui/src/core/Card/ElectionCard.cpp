#include "ElectionCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"

ElectionCard::ElectionCard()
{
}

ElectionCard::ElectionCard(const string &type, const string &description, int amountPerPlayer) : GeneralFundCard(type, description), amountPerPlayer(amountPerPlayer)
{
}

ElectionCard::~ElectionCard()
{
}

void ElectionCard::execute(Player &p, GameState &gs)
{
    vector<Player *> players = gs.getActivePlayers();
    for (Player *other : players)
    {
        if (other == &p)
            continue;
        p -= (int)amountPerPlayer;
        *other += (int)amountPerPlayer;
    }
}