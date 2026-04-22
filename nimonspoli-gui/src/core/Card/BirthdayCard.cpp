#include "BirthdayCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"

BirthdayCard::BirthdayCard()
{
}

BirthdayCard::BirthdayCard(const string &type, const string &description, int amountPerPlayer) : GeneralFundCard(type, description), amountPerPlayer(amountPerPlayer)
{
}

BirthdayCard::~BirthdayCard()
{
}

void BirthdayCard::execute(Player &p, GameState &gs)
{
    vector<Player *> players = gs.getActivePlayers();
    for (Player *other : players)
    {
        if (other == &p)
            continue;
        *other -= amountPerPlayer;
        p += amountPerPlayer;
    }
}